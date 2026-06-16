#pragma once
#include <GLFW/glfw3.h>
#include "FlythroughCamera.hpp"
#include "ArcballCamera.hpp"

enum class CameraMode { Flythrough, Arcball };

class InputManager {
public:
    void init(GLFWwindow* window) {
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window,         keyCallback);
        glfwSetCursorPosCallback(window,   mouseCallback);
        glfwSetScrollCallback(window,      scrollCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);

        // start with cursor locked for flythrough
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void setFlythroughCamera(FlythroughCamera* cam) { flythrough = cam; }
    void setArcballCamera(ArcballCamera* cam)        { arcball    = cam; }

    CameraMode getMode() const { return mode; }

    // returns whichever camera is active
    Camera* getActiveCamera() const {
        return mode == CameraMode::Flythrough
            ? (Camera*)flythrough
            : (Camera*)arcball;
    }

private:
    FlythroughCamera* flythrough  = nullptr;
    ArcballCamera*    arcball     = nullptr;
    CameraMode        mode        = CameraMode::Flythrough;

    float lastX      = 0.0f;
    float lastY      = 0.0f;
    bool  firstMouse = true;
    bool  orbiting   = false;   // left mouse held  (arcball only)
    bool  panning    = false;   // middle mouse held (arcball only)

    // Tab switches between cameras
    void switchMode(GLFWwindow* window) {
        if (mode == CameraMode::Flythrough) {
            mode = CameraMode::Arcball;
            // free cursor for arcball clicking
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            mode = CameraMode::Flythrough;
            // lock cursor for flythrough
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        firstMouse = true; // prevent jump on switch
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* self    = (InputManager*)glfwGetWindowUserPointer(window);
        bool  pressed = (action == GLFW_PRESS || action == GLFW_REPEAT);

        // Tab — switch camera mode
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
            self->switchMode(window);
            return;
        }

        // arrow keys — flythrough only
        if (self->mode == CameraMode::Flythrough && self->flythrough) {
            if (key == GLFW_KEY_UP)    self->flythrough->setKey(0, pressed);
            if (key == GLFW_KEY_DOWN)  self->flythrough->setKey(1, pressed);
            if (key == GLFW_KEY_LEFT)  self->flythrough->setKey(2, pressed);
            if (key == GLFW_KEY_RIGHT) self->flythrough->setKey(3, pressed);
        }
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* self = (InputManager*)glfwGetWindowUserPointer(window);

        // mouse buttons only used in arcball mode
        if (self->mode == CameraMode::Arcball) {
            if (button == GLFW_MOUSE_BUTTON_LEFT)
                self->orbiting = (action == GLFW_PRESS);

            if (button == GLFW_MOUSE_BUTTON_RIGHT)
                self->panning = (action == GLFW_PRESS);

            if (action == GLFW_PRESS)
                self->firstMouse = true;
        }

    }

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* self = (InputManager*)glfwGetWindowUserPointer(window);

        if (self->firstMouse) {
            self->lastX      = xpos;
            self->lastY      = ypos;
            self->firstMouse = false;
            return;
        }

        float xOffset =  (xpos - self->lastX);
        float yOffset = -(ypos - self->lastY);
        self->lastX = xpos;
        self->lastY = ypos;

        if (self->mode == CameraMode::Flythrough && self->flythrough) {
            // flythrough — mouse always looks around
            self->flythrough->processMouseMove(xOffset, yOffset);
        }

        if (self->mode == CameraMode::Arcball && self->arcball) {
            // arcball — only move on button held
            if (self->orbiting) self->arcball->processOrbit(xOffset, yOffset);
            if (self->panning)  self->arcball->processPan(xOffset, yOffset);
        }
    }

    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
        auto* self = (InputManager*)glfwGetWindowUserPointer(window);

        // scroll only zooms arcball
        if (self->mode == CameraMode::Arcball && self->arcball)
            self->arcball->processZoom((float)yOffset);
    }
};
