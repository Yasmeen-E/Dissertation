#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "Mesh.hpp"
#include "Texture.hpp"



struct DrawCall {
    std::shared_ptr<Mesh>    mesh;
    std::shared_ptr<Texture> diffuse;   // albedo/colour texture
};

class Model {
public:
    Model(const char *fileOBJname, const char *fileTexname);

    void draw(GLuint shader) const;
    void drawOcclude(GLuint shader) const;

    glm::mat4 transform = glm::mat4(1.0f);


private:
    std::vector<DrawCall> drawCalls;

    void load(const char *fileOBJname, const char *fileTexname);
};
