# 项目状态

最后更新：2026-07-29

## 当前阶段

Phase 1：单机 FPS 基础

## 当前能力

- 可以创建 OpenGL 3.3 Core Profile 窗口，运行事件循环并响应 framebuffer 尺寸变化。
- 已通过 GLAD 加载现代 OpenGL API，可使用 VAO、VBO 和 Shader Program 绘制橙色三角形。
- `GlfwRuntime`、`Window` 和 `ShaderProgram` 使用 RAII 管理资源生命周期。
- `ShaderProgram` 支持 Shader 编译、Program 链接、错误日志和无效状态检查。
- `ShaderProgram` 支持按名称上传 `mat4` uniform。
- 可以使用 GLM 模型矩阵对物体应用平移变换，并在顶点着色器中完成顶点变换。
- 顶点着色器已使用 `projection * view * model` 完成完整的 MVP 变换。
- 已加入固定观察矩阵和透视投影，能够从位于 `(0, 0, 3)` 的观察点显示场景。
- 摄像机观察矩阵由位置、朝向和上方向通过 `glm::lookAt` 生成。
- `Window` 隐藏原生 GLFW 句柄，并提供按键状态查询。
- 摄像机支持使用 W/S/A/D 进行帧率无关的前后与横向移动。
- GLFW 3.4 与 GLM 1.0.3 已通过 CMake `FetchContent` 接入并固定版本。

## 当前任务

使用鼠标或触摸板改变 FPS 摄像机朝向。

验收标准：

- `Window` 在不暴露 GLFW 句柄的前提下提供光标位置和捕获控制。
- 能说明 yaw、pitch 和摄像机朝向向量之间的关系。
- 水平和垂直滑动触摸板可以连续改变观察方向，且不会发生视角翻转。

## 后续任务

1. 增加退出光标捕获或键盘视角控制等调试操作。
2. 处理窗口宽高比变化。
3. 加入可提供空间参照的简单 3D 场景。

## 当前实现决策

- 使用 OpenGL 3.3 Core Profile；macOS 启用 forward compatibility。
- GLAD 生成代码直接纳入仓库并构建为独立静态库。
- GLFW 和 GLM 通过 `FetchContent` 获取并固定版本。
- 拥有 OpenGL 或 GLFW 资源的对象采用 RAII，并禁止复制。
- Shader 链接完成后释放；Program 由 `ShaderProgram` 持有到使用结束。
- C/C++ 格式基于 LLVM 风格：4 空格缩进、100 列限制、左侧指针对齐。

## 已知问题或阻塞

- 暂无。
