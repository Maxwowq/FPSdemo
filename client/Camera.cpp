#include "Camera.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

// 输入camera的初始位置和朝向，以及up，速率和灵敏度
Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp, float sensitivity)
    : position_(position), front_(glm::normalize(front)), worldUp_(worldUp),
      sensitivity_(sensitivity) {
    // 计算初始的yaw和pitch
    yaw_ = glm::degrees(atan2(front_.z, front_.x));
    pitch_ = glm::degrees(asin(front_.y));
    // 限制pitch的范围
    if (pitch_ > 89.0F) {
        pitch_ = 89.0F;
    } else if (pitch_ < -89.0F) {
        pitch_ = -89.0;
    }
}

// 根据鼠标的xy位移计算镜头的front_
void Camera::rotation(double xShift, double yShift) {
    // 计算新的yaw和pitch
    yaw_ += xShift * sensitivity_;
    pitch_ += yShift * sensitivity_;

    // 确保pitch在-89到89之间
    if (pitch_ > 89.0) {
        pitch_ = 89.0;
    } else if (pitch_ < -89.0) {
        pitch_ = -89.0;
    }
    // 转换为radians
    const double yawRad = glm::radians(yaw_);
    const double pitchRad = glm::radians(pitch_);
    // 计算新的front
    float frontX = static_cast<float>(cos(yawRad) * cos(pitchRad));
    float frontZ = static_cast<float>(sin(yawRad) * cos(pitchRad));
    float frontY = static_cast<float>(sin(pitchRad));
    // 更新front_
    front_ = glm::normalize(glm::vec3{frontX, frontY, frontZ});
}

// 返回镜头的view矩阵
glm::mat4 Camera::viewMat() const {
    return glm::lookAt(position_, position_ + front_, worldUp_);
}

// 提供camera当前的position
glm::vec3 Camera::position() const {
    return position_;
}

// 提供camera的forward方向（即忽略y分量的前方）
glm::vec3 Camera::forwardOnGround() const {
    glm::vec3 forward{front_.x, 0.0F, front_.z};
    return glm::normalize(forward);
}

// 提供camera的right方向
glm::vec3 Camera::rightOnGround() const {
    return glm::normalize(glm::cross(front_, worldUp_));
}

// 根据位移移动
void Camera::move(const glm::vec3& displacement) {
    position_ += displacement;
}

// 提供当前的front朝向
glm::vec3 Camera::front() const {
    return front_;
}
