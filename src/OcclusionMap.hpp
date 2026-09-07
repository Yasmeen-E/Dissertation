#pragma once

#include <glm/glm.hpp>

struct ShadowStruct
{
	unsigned int FBO;
	unsigned int Texture;
};



ShadowStruct setup_shadowmap(int w, int h)
{
	ShadowStruct shadow;

	glGenFramebuffers(1, &shadow.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
	glGenTextures(1, &shadow.Texture);
	glBindTexture(GL_TEXTURE_2D, shadow.Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER,shadow.FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,shadow.Texture,0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	return shadow;
}



//orthogonal projection straight down
glm::mat4 getOrtho()
{
    // Position high above the scene, looking straight down

    glm::vec3 position  = glm::vec3(0.f, 30.f, 0.f);  
    glm::vec3 target    = glm::vec3(0.f,  0.f, 0.f);  
    glm::vec3 up        = glm::vec3(0.f, 0.f, 1.f);

    float orthoSize     = 40.f;
    float nearPlane     = 1.f;
    float farPlane      = 200.f;

    glm::mat4 lightProjection = glm::ortho(
        -orthoSize, orthoSize,   // left, right
        -orthoSize, orthoSize,   // bottom, top
         nearPlane, farPlane
    );

    glm::mat4 lightView = glm::lookAt(position, target, up);

    return lightProjection * lightView;
}



glm::mat4 getOrtho(const glm::vec3& Direction, const glm::vec3& sceneCenter = glm::vec3(0.f))
{

	
    glm::vec3 dir = glm::normalize(Direction);

    float orthoSize  = 30.f;
    float nearPlane  = 1.f;
    float farPlane   = 200.f;
    float distance   = 100.f; 

    glm::vec3 position = sceneCenter - dir * distance;

    // Pick an up vector that isn't parallel to the light direction
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    if (glm::abs(glm::dot(dir, up)) > 0.999f)
        up = glm::vec3(0.f, 0.f, 1.f);

    glm::mat4 lightView = glm::lookAt(position, sceneCenter, up);

    glm::mat4 lightProjection = glm::ortho(
        -orthoSize, orthoSize,
        -orthoSize, orthoSize,
        nearPlane, farPlane
    );

    return lightProjection * lightView;
}

