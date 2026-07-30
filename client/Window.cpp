#include "Window.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

Window::Window(int width, int height, const char* title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    handle_ = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (handle_ != nullptr) {
        glfwSetFramebufferSizeCallback(handle_, framebufferSizeCallback);
    }
}

Window::~Window() {
    if (handle_ != nullptr) {
        glfwDestroyWindow(handle_);
    }
}

bool Window::isValid() const {
    return handle_ != nullptr;
}

void Window::makeContextCurrent() {
    glfwMakeContextCurrent(handle_);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(handle_) == GLFW_TRUE;
}

void Window::swapBuffers() {
    glfwSwapBuffers(handle_);
}

// 注册给glfwSetFramebufferSizeCallback()的静态函数（不访问成员变量），参数中的第一个不使用故仅写类型不写名称
void Window::framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

// 更新OpenGL的绘图像素
void Window::updateViewport() {
    int width, height;
    // 调用glfw接口获取像素尺寸
    glfwGetFramebufferSize(handle_, &width, &height);
    // 设置 viewport 的 x 为 0，y 为 0，宽为 width，高为 height
    glViewport(0, 0, width, height);
}

// 判断按键是否按下
bool Window::isKeyPressed(int key) const {
    return glfwGetKey(handle_, key) == GLFW_PRESS;
}

// 设置鼠标mode
void Window::setCursorDisabled() {
    glfwSetInputMode(handle_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// 设置鼠标为normal
void Window::setCursorNormal() {
    glfwSetInputMode(handle_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// 查询光标位置
void Window::getCursorPos(double& xPosition, double& yPosition) const {
    glfwGetCursorPos(handle_, &xPosition, &yPosition);
}
