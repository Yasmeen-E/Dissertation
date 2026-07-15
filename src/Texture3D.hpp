#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include <vector>
#include <cstdlib>
#include <GL/glew.h> 

//generate random 3D texture for perlin noise later 
GLuint createPerlinTexture3D(int size) {
    std::vector<unsigned char> data(size * size * size * 3);



    for (int z = 0; z < size; z++) {
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int index = (z * size * size + y * size + x) * 3;

                    // 3 independent Perlin values
                    float fx = (float)x / size;
                    float fy = (float)y / size;
                    float fz = (float)z / size;

                    float r = stb_perlin_noise3(fx * 4.0f, fy * 4.0f, fz * 4.0f, 4, 4, 4);
                    float g = stb_perlin_noise3(fx * 4.0f + 5.2f, fy * 4.0f + 1.3f, fz * 4.0f + 8.1f, 4, 4, 4);
                    float b = stb_perlin_noise3(fx * 4.0f + 9.7f, fy * 4.0f + 3.9f, fz * 4.0f + 2.4f, 4, 4, 4);

                    // remap from  to [0,1] 
                    data[index + 0] = static_cast<unsigned char>((r * 0.5f + 0.5f) * 255.0f);
                    data[index + 1] = static_cast<unsigned char>((g * 0.5f + 0.5f) * 255.0f);
                    data[index + 2] = static_cast<unsigned char>((b * 0.5f + 0.5f) * 255.0f);

            }
        }
    }

  
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_3D, tex);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, size, size, size, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data.data());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_3D, 0);
    return tex;
}
