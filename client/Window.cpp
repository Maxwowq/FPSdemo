#include "Window.h"
#include <GLFW/glfw3.h>

Window::Window(int width, int height, const char* title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    handle_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
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
