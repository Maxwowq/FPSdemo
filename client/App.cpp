#include "App.h"
#include "Camera.h"
#include "GlfwRuntime.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/gl.h>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <span>

namespace {
struct Box {
    glm::vec3 center;
    glm::vec3 size{1.0F, 1.0F, 1.0F};
    glm::vec3 color;
};

void drawBox(const ShaderProgram& program, const Box& box, const Mesh& boxMesh,
             const Mesh& lineMesh) {
    glm::mat4 model{1.0F};
    model = glm::translate(model, box.center);
    model = glm::scale(model, box.size);
    program.setMat4("model", model);
    program.setVec3("objectColor", box.color);

    boxMesh.drawArrays();

    program.setVec3("objectColor", {0.0F, 0.0F, 0.0F});
    lineMesh.drawArrays();
}

struct Aabb {
    glm::vec3 min;
    glm::vec3 max;
};

bool overlaps(const Aabb& a, const Aabb& b) {
    return a.max.x > b.min.x && a.min.x < b.max.x && a.max.y > b.min.y && a.min.y < b.max.y &&
           a.max.z > b.min.z && a.min.z < b.max.z;
}

// 从box结构体制造Aabb
Aabb makeAabb(const Box& box) {
    return Aabb{box.center - box.size / 2.0F, box.center + box.size / 2.0F};
}

// 从摄像机位置制造玩家Aabb，玩家的size固定
Aabb makePlayerAabb(const glm::vec3& cameraPosition) {
    glm::vec3 min = cameraPosition + glm::vec3{-0.3F, -1.0F, -0.3F};
    glm::vec3 max = cameraPosition + glm::vec3{0.3F, 0.8F, 0.3F};
    return Aabb{min, max};
}

// 遍历所有box判断是否有重叠
bool overlapsAnyBox(const Aabb& playerAabb, std::span<const Box> boxes) {
    for (const Box& box : boxes) {
        const Aabb boxAabb = makeAabb(box);
        if (overlaps(boxAabb, playerAabb)) {
            return true;
        }
    }

    return false;
}

// 处理碰撞位移
glm::vec3 resolveHorizontalDisplacement(glm::vec3 displacement, const glm::vec3& cameraPosition,
                                        std::span<const Box> boxes) {
    // 先尝试x方向位移
    Aabb playerAabb = makePlayerAabb(cameraPosition + glm::vec3{displacement.x, 0.0F, 0.0F});
    // 若发生碰撞
    if (overlapsAnyBox(playerAabb, boxes)) {
        // 取消x方向位移
        displacement.x = 0.0F;
    }
    // 再尝试结合z方向位移
    playerAabb = makePlayerAabb(cameraPosition + displacement);
    // 若发生碰撞
    if (overlapsAnyBox(playerAabb, boxes)) {
        // 取消z方向位移
        displacement.z = 0.0F;
    }

    // 返回处理后的位移
    return displacement;
}

// 拆分子步法计算碰撞位移
glm::vec3 miniStepDisplacement(glm::vec3 displacement, glm::vec3 cameraPosition,
                               std::span<const Box> boxes, const float stepLength = 0.1F) {
    // 计算拆分数量
    int stepNum = static_cast<int>(glm::length((displacement) / stepLength)) + 1;
    // 拆分位移
    glm::vec3 miniDisplacement = displacement / static_cast<float>(stepNum);

    // 逐步计算
    glm::vec3 finalDisplacement{0.0F};
    for (int step = 0; step < stepNum; step++) {
        glm::vec3 stepDisplacement =
            resolveHorizontalDisplacement(miniDisplacement, cameraPosition, boxes);
        cameraPosition += stepDisplacement;
        finalDisplacement += stepDisplacement;
    }

    return finalDisplacement;
}

} // namespace

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
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置刷新率
    glfwSwapInterval(1);

    // clang-format off
    // 立方体顶点数组
    const float vertices[] = {
        // 前面
        -0.5F, -0.5F, 0.5F, 0.5F,  -0.5F, 0.5F, 0.5F,  0.5F,  0.5F,
        0.5F,  0.5F,  0.5F, -0.5F, 0.5F,  0.5F, -0.5F, -0.5F, 0.5F,

        // 后面
        -0.5F, -0.5F, -0.5F, -0.5F, 0.5F,  -0.5F, 0.5F,  0.5F,  -0.5F,
        0.5F,  0.5F,  -0.5F, 0.5F,  -0.5F, -0.5F, -0.5F, -0.5F, -0.5F,

        // 左面
        -0.5F, 0.5F,  0.5F,  -0.5F, 0.5F,  -0.5F, -0.5F, -0.5F, -0.5F,
        -0.5F, -0.5F, -0.5F, -0.5F, -0.5F, 0.5F,  -0.5F, 0.5F,  0.5F,

        // 右面
        0.5F,  0.5F,  0.5F,  0.5F,  -0.5F, 0.5F,  0.5F,  -0.5F, -0.5F,
        0.5F,  -0.5F, -0.5F, 0.5F,  0.5F,  -0.5F, 0.5F,  0.5F,  0.5F,

        // 上面
        -0.5F, 0.5F, -0.5F, -0.5F, 0.5F, 0.5F, 0.5F, 0.5F, 0.5F,
        0.5F,  0.5F, 0.5F,  0.5F,  0.5F, -0.5F, -0.5F, 0.5F, -0.5F,

        // 下面
        -0.5F, -0.5F, -0.5F, 0.5F,  -0.5F, -0.5F, 0.5F,  -0.5F, 0.5F,
        0.5F,  -0.5F, 0.5F,  -0.5F, -0.5F, 0.5F,  -0.5F, -0.5F, -0.5F,
    };
    // 立方体描边数组
    const float lines[] = {
        // 连接前后两个面的四条边
        0.5F,  0.5F,  0.5F,  0.5F,  0.5F,  -0.5F,
        0.5F,  -0.5F, 0.5F,  0.5F,  -0.5F, -0.5F,
        -0.5F, 0.5F,  0.5F,  -0.5F, 0.5F,  -0.5F,
        -0.5F, -0.5F, 0.5F,  -0.5F, -0.5F, -0.5F,

        // 前面的四条边
        -0.5F, -0.5F, 0.5F,  0.5F,  -0.5F, 0.5F,
        0.5F,  -0.5F, 0.5F,  0.5F,  0.5F,  0.5F,
        0.5F,  0.5F,  0.5F,  -0.5F, 0.5F,  0.5F,
        -0.5F, 0.5F,  0.5F,  -0.5F, -0.5F, 0.5F,

        // 后面的四条边
        -0.5F, -0.5F, -0.5F, 0.5F,  -0.5F, -0.5F,
        0.5F,  -0.5F, -0.5F, 0.5F,  0.5F,  -0.5F,
        0.5F,  0.5F,  -0.5F, -0.5F, 0.5F,  -0.5F,
        -0.5F, 0.5F,  -0.5F, -0.5F, -0.5F, -0.5F,
    };
    // clang-format on

    Mesh boxMesh{vertices, GL_TRIANGLES};
    Mesh lineMesh{lines, GL_LINES};

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
        uniform vec3 objectColor;
        void main(){
            fragmentColor = vec4(objectColor, 1.0);
        }
    )";

    // 利用shader源码创建program
    ShaderProgram program(vertexShaderSource, fragmentShaderSource);
    // 若创建失败，则结束
    if (!program.isValid()) {
        return -1;
    }

    // 立方体网格以局部原点为中心，Box 的 center 和 size 可同时用于渲染与碰撞。
    const Box boxes[] = {
        {{0.0F, 0.0F, -0.5F}, {1.0F, 1.0F, 1.0F}, {1.00F, 0.35F, 0.20F}},
        {{1.5F, 0.0F, -2.5F}, {1.0F, 1.0F, 1.0F}, {0.20F, 0.65F, 1.00F}},
        {{-1.5F, 0.5F, -4.5F}, {1.0F, 1.0F, 1.0F}, {0.35F, 0.85F, 0.40F}},
        {{0.0F, -1.5F, -6.5F}, {1.0F, 1.0F, 1.0F}, {0.85F, 0.35F, 0.90F}},
        {{2.0F, 1.0F, -8.5F}, {1.0F, 1.0F, 1.0F}, {1.00F, 0.80F, 0.20F}},
    };
    // 保持原有地面的世界空间范围：X [-50, 50]、Y [-3, -2]、Z [-100, 0]。
    const Box ground{{0.0F, -2.5F, -50.0F}, {100.0F, 1.0F, 100.0F}, {0.22F, 0.27F, 0.24F}};

    // 摄像机位置
    glm::vec3 cameraPosition{0.0F, -1.0F, 3.0F};
    // 摄像机朝向（非坐标，而是向量）
    glm::vec3 cameraFront{0.0F, 0.0F, -1.0F};
    // 世界上方（向量）
    glm::vec3 worldUp{0.0F, 1.0F, 0.0F};
    // 每秒移动距离
    const float cameraSpeed = 2.5F;
    // 灵敏度
    const float sensitivity = 0.1F;
    // 创建摄像机对象
    Camera camera{cameraPosition, cameraFront, worldUp, sensitivity};

    // 启用shader program
    program.use();

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

            // 初始化移动方向
            glm::vec3 moveDirection{0.0F};
            // 计算累积移动方向
            if (window.isKeyPressed(GLFW_KEY_W)) {
                moveDirection += camera.forwardOnGround();
            }
            if (window.isKeyPressed(GLFW_KEY_S)) {
                moveDirection -= camera.forwardOnGround();
            }
            if (window.isKeyPressed(GLFW_KEY_D)) {
                moveDirection += camera.rightOnGround();
            }
            if (window.isKeyPressed(GLFW_KEY_A)) {
                moveDirection -= camera.rightOnGround();
            }
            // 若移动方向非零
            if (moveDirection != glm::vec3{0.0F}) {
                // 计算位移
                glm::vec3 displacement = cameraSpeed * deltaTime * glm::normalize(moveDirection);
                // 处理碰撞
                displacement = miniStepDisplacement(displacement, camera.position(), boxes);
                // 移动相机
                camera.move(displacement);
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

        // 设置清除颜色
        glClearColor(0.1F, 0.15F, 0.2F, 1.0F);
        // 清除颜色和深度缓存
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 获取frambuffersize
        int width, height;
        window.getFramebufferSize(width, height);
        // 仅在height和width大于0的情况绘图
        if (height > 0 && width > 0) {
            const float aspect = static_cast<float>(width) / static_cast<float>(height);

            // 创建projection矩阵
            glm::mat4 projection = glm::perspective(glm::radians(45.0F), aspect, 0.1F, 100.0F);
            // 上传projection矩阵
            program.setMat4("projection", projection);

            for (const Box& box : boxes) {
                drawBox(program, box, boxMesh, lineMesh);
            }

            drawBox(program, ground, boxMesh, lineMesh);
        }

        window.swapBuffers();
    }

    return 0;
}
