#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Base Class for Camera 
 * 
 */
class Camera {
public:
    Camera(float width, float height)   
        : aspect(width / height)
        , position(10.0f, 10.0f, 10.0f)
        , front(0.0f, 0.0f, -1.0f)
        , up(0.0f, 1.0f, 0.0f)
        , worldUp(0.0f, 1.0f, 0.0f)
        , yaw(-90.0f)
        , pitch(0.0f)
        , fov(60.0f)
        , speed(5.0f)
        , sensitivity(0.1f)
    {
        updateVectors();
    }

    void resize(float width, float height)  { aspect = width / height; }

    //getters 
    glm::vec3 getPosition() const{ return position ;}
    glm::vec3 getForward() const { return front; }
    glm::vec3 getRight() const { return right; }
    float getSpeed() const { return speed; }
    float getSensitivity() const { return sensitivity; }
    float getYaw() const {return yaw; }
    float getPitch() const { return pitch; }

    //setters
    void setYaw(float y) { yaw = y; }
    void setPosition(glm::vec3 pos) { position.x = pos.x; position.y = pos.y; position.z = pos.z;}
    void setPitch(float p) { pitch = p; }


    void updateVectors()
    {

        glm::vec3 f;

        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front)); 

    };

    virtual glm::mat4 getView() const{ return glm::lookAt(position, position + front, up);}

    glm::mat4 getProj() const{
    glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, 0.1f, 1000.f);
    return proj;
    }


    virtual ~Camera() = default;


protected:

  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 right;
  glm::vec3 up;
  glm::vec3 worldUp;

  float yaw;
  float pitch;

  float aspect;
  float fov;


  float speed; // 5 units per second
  float sensitivity;
    
};
