#include "parseOBJ.hpp"




Mesh readOBJ(const char* filename) {
  Mesh outputMesh;

  // parse file
  rpd::Result res =
      rpd::ParseFile(filename,
                     rpd::MaterialLibrary::Default());

  if (res.error) {
    std::cout << "failed to parse OBJ in mesh manager: "
              << res.error.code.message() << std::endl;
    return outputMesh;
  }

  // read file information (emplace back)
  rpd::Triangulate(res);

  // check the properites of the material? (for the mesh)

  // check the remaining properties (normals, vertices, faces etc)
  for (auto const &shape : res.shapes) {
    for(auto const& index : shape.mesh.indices ){

      outputMesh.meshPositions.emplace_back(
					    glm::vec3(
						      res.attributes.positions[3 * index.position_index + 0],
						      res.attributes.positions[3 * index.position_index + 1],
						      res.attributes.positions[3 * index.position_index + 2]
						      )
					    );

      outputMesh.meshTexcoords.emplace_back(
					    glm::vec2(
					    res.attributes.texcoords[2 * index.texcoord_index + 0],
					    res.attributes.texcoords[2 * index.texcoord_index + 1]
						      )
					    );

      outputMesh.meshNormals.emplace_back(
					  glm::vec3(
					  res.attributes.normals[3 * index.normal_index + 0],
					  res.attributes.normals[3 * index.normal_index + 1],
					  res.attributes.normals[3 * index.normal_index + 2]
						    )
					  );
    }
  }

  std::cout << "parse successful!" << std::endl;

  return outputMesh;

}
