#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

class Mesh {
public:
    Mesh(std::vector<Vertex>& vertices);
    ~Mesh();

    void draw() const;

    GLuint VAO, VBO;
    unsigned int vertexCount;

private:
    void setup();
    std::vector<Vertex>vertices;

};
