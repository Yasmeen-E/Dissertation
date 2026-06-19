#pragma once
#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture(const char* filename) ;
    ~Texture();

    void bind(unsigned int slot = 0) const;

    GLuint texObject;
};
