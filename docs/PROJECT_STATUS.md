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
- GLFW 3.4 与 GLM 1.0.3 已通过 CMake `FetchContent` 接入并固定版本。

## 当前任务

区分模型、观察和投影矩阵的职责，为后续加入基础观察矩阵和透视投影做准备。

验收标准：

- 能说明模型、观察和投影矩阵分别改变什么。
- 能说明三种矩阵的组合顺序。
- 明确下一步需要传入顶点着色器的矩阵及其职责。

## 后续任务

1. 加入基础观察矩阵和透视投影。
2. 在此基础上开始 FPS 摄像机。

## 当前实现决策

- 使用 OpenGL 3.3 Core Profile；macOS 启用 forward compatibility。
- GLAD 生成代码直接纳入仓库并构建为独立静态库。
- GLFW 和 GLM 通过 `FetchContent` 获取并固定版本。
- 拥有 OpenGL 或 GLFW 资源的对象采用 RAII，并禁止复制。
- Shader 链接完成后释放；Program 由 `ShaderProgram` 持有到使用结束。
- C/C++ 格式基于 LLVM 风格：4 空格缩进、100 列限制、左侧指针对齐。

## 已知问题或阻塞

- 暂无。
