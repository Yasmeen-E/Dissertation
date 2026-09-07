#pragma once
#include "Camera.hpp"
#include <GLFW/glfw3.h>

class FlythroughCamera : public Camera {
public:
    FlythroughCamera(float width, float height)
        : Camera(width, height)
    {
        updateVectors();
    }

    // call every frame
    void update(float deltaTime) {

        //shift increases speed 
        float boost    = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 10.0f : 1.0f;
        float velocity = speed * deltaTime * boost;

        if (keys[0]) position += front * velocity;  // up arrow    - forward
        if (keys[1]) position -= front * velocity;  // down arrow  - backward
        if (keys[2]) position -= right * velocity;  // left arrow  - left
        if (keys[3]) position += right * velocity;  // right arrow - right
    }


    void processMouseMove(float xOffset, float yOffset) {
        yaw   += xOffset * sensitivity;
        pitch += yOffset * sensitivity;
        pitch  = glm::clamp(pitch, -89.0f, 89.0f);
        updateVectors();
    }

    // called from key callback
    void setKey(int index, bool pressed) {
        if (index >= 0 && index < 4)
            keys[index] = pressed;
    }

private:
    bool keys[4] = { false, false, false, false };
    // 0 = forward, 1 = backward, 2 = left, 3 = right
};
