#include "App.h"
#include "GlfwRuntime.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glad/gl.h>
#include <iostream>
#include <string>

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
    // 创建shader
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 存入源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    // 编译shader
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    // 查询编译结果
    GLint vertexCompileStatus = GL_FALSE;
    GLint fragmentCompileStatus = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompileStatus);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompileStatus);
    // 若编译失败
    if (vertexCompileStatus != GL_TRUE) {
        // 查询错误日志字符空间长度
        GLint logLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        // 分配缓存空间
        std::string logBuffer(static_cast<std::size_t>(logLength), '\0');
        glGetShaderInfoLog(vertexShader, logLength, nullptr, logBuffer.data());
        // 输出日志
        std::cerr << "Vertex Shader Compilation Failed:\n" << logBuffer;
        // 结束运行，返回异常
        return -1;
    }
    if (fragmentCompileStatus != GL_TRUE) {
        // 查询错误日志字符空间长度
        GLint logLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        // 分配缓存空间
        std::string logBuffer(static_cast<std::size_t>(logLength), '\0');
        glGetShaderInfoLog(fragmentShader, logLength, nullptr, logBuffer.data());
        // 输出日志
        std::cerr << "Fragment Shader Compilation Failed:\n" << logBuffer;
        // 结束运行，返回异常
        return -1;
    }

    // 创建Program
    const GLuint program = glCreateProgram();
    // 装入shader
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    // 链接program
    glLinkProgram(program);

    //  查询program链接状态
    GLint programStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &programStatus);
    // 若链接失败
    if (programStatus != GL_TRUE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::string logBuffer(static_cast<size_t>(logLength), '\0');
        glGetProgramInfoLog(program, logLength, nullptr, logBuffer.data());
        std::cerr << "Shader Program Link Failed:\n" << logBuffer;
        return -1;
    }

    // 若链接无误，则可以释放shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 启用shader program
    glUseProgram(program);

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

    // 释放program
    glDeleteProgram(program);

    // 释放vao和vbo对象
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return 0;
}
