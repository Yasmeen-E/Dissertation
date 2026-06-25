#include "Model.hpp"
#include<iostream>
#include<stdio.h>
#include <stdlib.h>



#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


void Model::load(const char *filename, const char *fileTexturename) {


   //load in Texture gonna be done albedo for now
  auto tex = std::make_shared<Texture>(fileTexturename);


  tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename))
	{
		throw std::runtime_error(warn + err);
	}


  for (const auto& shape : shapes)
	{

    std::vector<Vertex> vertices;
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vert;

      vert.position =
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			if (index.texcoord_index >= 0) {
				vert.texCoord =
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]

				};
			}

			vert.normal =
			{
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};


      vertices.push_back(vert);

    }

     DrawCall dc;
    dc.mesh    = std::make_shared<Mesh>(vertices);
    dc.diffuse = tex;
    drawCalls.push_back(dc);
  
  }


   
  std::cout << "parse successful!" << std::endl;

}

Model::Model(const char *fileOBJname,const char *fileTexname) {
    load(fileOBJname, fileTexname);
}

void Model::draw(GLuint shader) const
{
  // upload transform matrix
  GLint modelLoc = glGetUniformLocation(shader, "Model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transform[0][0]);

  for (auto& dc : drawCalls) {
      if (dc.diffuse) {
          dc.diffuse->bind(1);
          glUniform1i(glGetUniformLocation(shader, "TextureSampler"), 1);
      }
      dc.mesh->draw();
  }

}

void Model::drawOcclude(GLuint shader) const
{
   GLint modelLoc = glGetUniformLocation(shader, "Model");
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transform[0][0]);
   
  for (auto& dc : drawCalls) {
      dc.mesh->draw();
  }
}
