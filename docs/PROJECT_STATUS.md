# 项目状态

最后更新：2026-07-29

## 当前阶段

Phase 1：单机 FPS 基础

## 当前能力

- 可以创建 OpenGL 3.3 Core Profile 窗口，运行事件循环并响应 framebuffer 尺寸变化。
- 已通过 GLAD 加载现代 OpenGL API，可使用 VAO、VBO 和 Shader Program 绘制橙色三角形。
- `GlfwRuntime`、`Window` 和 `ShaderProgram` 使用 RAII 管理资源生命周期。
- `ShaderProgram` 支持 Shader 编译、Program 链接、错误日志和无效状态检查。
- GLFW 3.4 与 GLM 1.0.3 已通过 CMake `FetchContent` 接入并固定版本。

## 当前任务

建立 `mat4` uniform 从 CPU 传入 Shader 的最小工作流，用模型矩阵平移三角形。

验收标准：

- `ShaderProgram` 提供最小的 `mat4` uniform 设置接口。
- 顶点着色器使用传入的模型矩阵。
- 运行时可以看到三角形产生预期平移。
- 修改后项目能够成功构建。

## 后续任务

1. 区分模型、观察和投影矩阵的职责。
2. 加入基础观察矩阵和透视投影。
3. 在此基础上开始 FPS 摄像机。

## 当前实现决策

- 使用 OpenGL 3.3 Core Profile；macOS 启用 forward compatibility。
- GLAD 生成代码直接纳入仓库并构建为独立静态库。
- GLFW 和 GLM 通过 `FetchContent` 获取并固定版本。
- 拥有 OpenGL 或 GLFW 资源的对象采用 RAII，并禁止复制。
- Shader 链接完成后释放；Program 由 `ShaderProgram` 持有到使用结束。
- C/C++ 格式基于 LLVM 风格：4 空格缩进、100 列限制、左侧指针对齐。

## 已知问题或阻塞

- 暂无。
