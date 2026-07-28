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
    // 先makeContextCurrent再update，因为OpenGL更新的是当前窗口
    window.updateViewport();
    glfwSwapInterval(1);

    while (!window.shouldClose()) {
        glfwPollEvents();

        glClearColor(0.1F, 0.15F, 0.2F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        window.swapBuffers();
    }

    return 0;
}
