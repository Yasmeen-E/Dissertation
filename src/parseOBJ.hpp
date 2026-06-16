#include "rapidobj.hpp"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

struct Mesh{
    glm::vec3 pos;
	glm::vec2 text;
	glm::vec3 nor;
}


class loadOBJ {
    Mesh readOBJ(const char* filename);
}


