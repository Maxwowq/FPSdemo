#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
  public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp, float speed, float sensitivity);

    glm::mat4 viewMat() const;

    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    void rotation(double xShift, double yShift);

  private:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 worldUp_;

    float speed_;
    float sensitivity_;

    double yaw_;
    double pitch_;

    glm::vec3 right_() const;
};
