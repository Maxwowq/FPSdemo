#include "App.h"
#include "GlfwRuntime.h"
#include "ShaderProgram.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

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

    // 使用glad查询注册OpenGL函数地址
    const int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        return -1;
    }

    // 先makeContextCurrent再update，因为OpenGL更新的是当前窗口
    window.updateViewport();
    glfwSwapInterval(1);

    const float vertices[] = {
        -0.5F, -0.5F, 0.0F, 0.5F, -0.5F, 0.0F, 0.0F, 0.5F, 0.0F,
    };

    // VAO和VBO对象
    GLuint vao = 0;
    GLuint vbo = 0;
    // 生成VAO和VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    // 绑定VAO，使当前VAO=vao
    glBindVertexArray(vao);
    // 绑定VBO，使GL_ARRAY_BUFFER=vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // 复制坐标数据到vbo中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 将读取规则记录到vao中
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    // 启动location:0顶点属性数组
    glEnableVertexAttribArray(0);

    // vertexshader源码
    constexpr const char* vertexShaderSource = R"(#version 330 core
        layout(location = 0) in vec3 aPosition;
        void main(){
            gl_Position = vec4(aPosition, 1.0);
        }
    )";
    // fragmentShader源码
    constexpr const char* fragmentShaderSource = R"(#version 330 core
        out vec4 fragmentColor;
        void main(){
            fragmentColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

    // 利用shader源码创建program
    ShaderProgram program(vertexShaderSource, fragmentShaderSource);
    // 若创建失败，则结束
    if (!program.isValid()) {
        return -1;
    }

    // 启用shader program
    program.use();

    while (!window.shouldClose()) {
        glfwPollEvents();

        glClearColor(0.1F, 0.15F, 0.2F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // 先绑定对应vao
        glBindVertexArray(vao);
        // 绘制命令
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.swapBuffers();
    }

    // 释放vao和vbo对象
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return 0;
}
