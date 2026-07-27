#include "GlfwRuntime.h"
#include <GLFW/glfw3.h>

GlfwRuntime::GlfwRuntime() {
    initialized_ = glfwInit() == GLFW_TRUE;
}

GlfwRuntime::~GlfwRuntime() {
    if (initialized_) {
        glfwTerminate();
    }
}

bool GlfwRuntime::isInitialized() const {
    return initialized_;
}
