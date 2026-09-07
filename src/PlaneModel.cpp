#include "PlaneModel.hpp"
#include <glm/gtc/type_ptr.hpp>

GroundPlane::GroundPlane(float size, int subdivisions, const char* texturePath)
{
    texture = std::make_shared<Texture>(texturePath); 
    generate(size, subdivisions);
  
}

GroundPlane::~GroundPlane()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void GroundPlane::generate(float size, int subdivisions)
{
    std::vector<GroundVertex> vertices;
    std::vector<unsigned int> indices;

    float half = size * 0.5f;
    float step = size / float(subdivisions);

    // Generate vertices
    for(int z = 0; z <= subdivisions; z++)
    {
        for(int x = 0; x <= subdivisions; x++)
        {
            GroundVertex v;
            v.position = glm::vec3(
                -half + x * step,
                0.0f,
                -half + z * step
            );
            v.texCoord = glm::vec2(
                float(x) / float(subdivisions),
                float(z) / float(subdivisions)
            );
            v.normal = glm::vec3(0.0f, 1.0f, 0.0f);  // flat up
            vertices.push_back(v);
        }
    }

    // Generate indices
    for(int z = 0; z < subdivisions; z++)
    {
        for(int x = 0; x < subdivisions; x++)
        {
            int topLeft     = z * (subdivisions + 1) + x;
            int topRight    = topLeft + 1;
            int bottomLeft  = (z + 1) * (subdivisions + 1) + x;
            int bottomRight = bottomLeft + 1;

            // Triangle 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Triangle 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    indexCount = indices.size();

    // Upload to GPU
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GroundVertex),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GroundVertex),
                          (void*)offsetof(GroundVertex, position));
    glEnableVertexAttribArray(0);

    // texcoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GroundVertex),
                          (void*)offsetof(GroundVertex, texCoord));
    glEnableVertexAttribArray(1);

    // normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GroundVertex),
                          (void*)offsetof(GroundVertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

}



void GroundPlane::draw(GLuint shader) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader, "Model"),
                       1, GL_FALSE, glm::value_ptr(transform));

    texture->bind(1); 
    glUniform1i(glGetUniformLocation(shader, "TextureSampler"), 1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GroundPlane::drawOcclude(GLuint shader) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader, "Model"),
                       1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
