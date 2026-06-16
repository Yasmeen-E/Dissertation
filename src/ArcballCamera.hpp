#pragma once
#include "Camera.hpp"

class ArcballCamera : public Camera {
public:
    ArcballCamera(float width, float height, glm::vec3 target = glm::vec3(0.0f))
        : Camera(width, height)
        , target(target)
        , radius(5.0f)
        , theta(0.0f)
        , phi(1.5f)
        , zoomSpeed(0.5f)
    {}

    // left mouse held + drag — orbit around target
    void processOrbit(float xOffset, float yOffset) {
        theta += xOffset * sensitivity;
        phi   -= yOffset * sensitivity;
        phi    = glm::clamp(phi, 0.1f, glm::pi<float>() - 0.1f);
    }

    // middle mouse held + drag — pan the target point
    void processPan(float xOffset, float yOffset) {
        glm::vec3 r      = glm::normalize(glm::cross(target - position, glm::vec3(0, 1, 0)));
        glm::vec3 u      = glm::normalize(glm::cross(r, target - position));
        float     panSpeed = radius * 0.001f;

        target -= r * xOffset * panSpeed;
        target += u * yOffset * panSpeed;
    }

    // scroll wheel — zoom
    void processZoom(float yOffset) {
        radius -= yOffset * zoomSpeed;
        radius  = glm::clamp(radius, 0.5f, 200.0f);
    }

    void update(float deltaTime) {
        position.x = target.x + radius * sin(phi) * cos(theta);
        position.y = target.y + radius * cos(phi);
        position.z = target.z + radius * sin(phi) * sin(theta);
    }

    glm::mat4 getView() const override {
        return glm::lookAt(position, target, glm::vec3(0, 1, 0));
    }

private:
    glm::vec3 target;
    float radius;
    float theta;
    float phi;
    float zoomSpeed;
};
