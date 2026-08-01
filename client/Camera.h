#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
  public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp, float sensitivity);

    glm::mat4 viewMat() const;

    void rotation(double xShift, double yShift);

    glm::vec3 position() const;
    glm::vec3 forwardOnGround() const;
    glm::vec3 rightOnGround() const;

    void move(const glm::vec3& displacement);

  private:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 worldUp_;

    float sensitivity_;

    double yaw_;
    double pitch_;
};
