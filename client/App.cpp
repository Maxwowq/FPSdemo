#include "App.h"
#include "GlfwRuntime.h"
#include "Window.h"
#include <GLFW/glfw3.h>

int App::run() {
    GlfwRuntime glfwRuntime;
    if (!glfwRuntime.isInitialized()) {
        return -1;
    }

    Window window{800, 600, "FPSdemo"};

    if (!window.isValid()) {
        return -1;
    }

    window.makeContextCurrent();
    glfwSwapInterval(1);

    while (!window.shouldClose()) {
        glfwPollEvents();
        window.swapBuffers();
    }

    return 0;
}
