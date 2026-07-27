#include "App.h"
#include <GLFW/glfw3.h>

int App::run(){
    if (glfwInit() == GLFW_FALSE){
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "FPSdemo", nullptr, nullptr);

    if (window == nullptr){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}