#include "App.h"
#include "Camera.h"
#include "GlfwRuntime.h"
#include "ShaderProgram.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/gl.h>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

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
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main(){
            gl_Position = projection * view * model * vec4(aPosition, 1.0);
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

    // 创建model矩阵
    glm::mat4 model{1.0F};
    // 平移x方向0.4F
    model = glm::translate(model, glm::vec3{0.4F, 0.0F, 0.0F});

    // 创建projection矩阵
    // 垂直视野角45，长宽比800/600，近裁剪0.1，远裁剪100
    glm::mat4 projection = glm::perspective(glm::radians(45.0F), 800.0F / 600.0F, 0.1F, 100.0F);

    // 摄像机位置
    glm::vec3 cameraPosition{0.0F, 0.0F, 3.0F};
    // 摄像机朝向（非坐标，而是向量）
    glm::vec3 cameraFront{0.0F, 0.0F, -1.0F};
    // 世界上方（向量）
    glm::vec3 worldUp{0.0F, 1.0F, 0.0F};
    // 每秒移动距离
    const float cameraSpeed = 2.5F;
    // 灵敏度
    const float sensitivity = 0.1F;
    // 创建摄像机对象
    Camera camera{cameraPosition, cameraFront, worldUp, cameraSpeed, sensitivity};

    // 启用shader program
    program.use();
    // 调用setM4上传矩阵
    program.setMat4("model", model);
    program.setMat4("projection", projection);

    // 初始时间记录
    double lastFrameTime = glfwGetTime();

    // 设置鼠标状态为隐藏
    window.setCursorDisabled();

    // 记录鼠标初始位置
    double lastCursorX, lastCursorY;
    window.getCursorPos(lastCursorX, lastCursorY);

    while (!window.shouldClose()) {
        glfwPollEvents();

        // 获取当前帧时间
        const double currentFrameTime = glfwGetTime();
        // 计算时间差
        const float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        // 更新lastFrameTime
        lastFrameTime = currentFrameTime;

        // 若当前cursor是隐藏状态
        if (window.isCursorDisabled()) {
            // 获取当前鼠标位置
            double currentCursorX, currentCursorY;
            window.getCursorPos(currentCursorX, currentCursorY);
            // 计算偏移量
            const double xShift = currentCursorX - lastCursorX;
            // Y轴反向计算，原因是窗口的Y定义和空间的Y定义反向
            const double yShift = lastCursorY - currentCursorY;
            // 更新鼠标位置
            lastCursorX = currentCursorX;
            lastCursorY = currentCursorY;

            // 转动相机
            camera.rotation(xShift, yShift);

            // 前后左右平移
            if (window.isKeyPressed(GLFW_KEY_W)) {
                camera.moveForward(deltaTime);
            }
            if (window.isKeyPressed(GLFW_KEY_S)) {
                camera.moveBackward(deltaTime);
            }
            if (window.isKeyPressed(GLFW_KEY_D)) {
                camera.moveRight(deltaTime);
            }
            if (window.isKeyPressed(GLFW_KEY_A)) {
                camera.moveLeft(deltaTime);
            }

            // 若按下ESC，则启用鼠标
            if (window.isKeyPressed(GLFW_KEY_ESCAPE)) {
                window.setCursorNormal();
            }
        }
        // 若在启用状态按下左键
        else if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
            // 则隐藏鼠标
            window.setCursorDisabled();
            // 重置鼠标起点位置
            window.getCursorPos(lastCursorX, lastCursorY);
        }

        // 导出view矩阵
        const glm::mat4 view = camera.viewMat();
        program.setMat4("view", view);

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
