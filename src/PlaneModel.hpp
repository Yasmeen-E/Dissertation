#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Texture.hpp"

struct GroundVertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

class GroundPlane {
public:
    GroundPlane(float size, int subdivisions, const char* texturePath);
    ~GroundPlane();
    void draw(GLuint shader) const;
    void drawOcclude(GLuint shader) const;
    glm::mat4 transform = glm::mat4(1.0f);

private:
    GLuint VAO, VBO, EBO;
    std::shared_ptr<Texture> texture; 
    int   indexCount;
    void generate(float size, int subdivisions);
};
