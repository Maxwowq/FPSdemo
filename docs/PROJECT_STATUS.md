# 项目状态

最后更新：2026-07-29

## 当前阶段

Phase 1：单机 FPS 基础

当前目标：在已完成的现代 OpenGL 渲染流程上引入 GLM 与基础空间变换，为 FPS 摄像机做准备。

## 已完成

- 明确项目目标、技术路线和五个开发阶段。
- 创建 `AGENTS.md`，保存长期项目约束和协作规则。
- 建立项目状态文档，用于在不同设备和 Codex 任务之间同步进度。
- 在 Windows 上安装并验证 MSVC Build Tools、Windows SDK、CMake 和 Ninja；C++20 最小程序可以正常编译、链接和运行。
- 创建最小 CMake 项目，使用 Ninja 和 x64 MSVC 成功构建并运行 `fps_client`。
- 将客户端拆分为 `main.cpp`、`App.h` 和 `App.cpp`，完成声明、定义、跨编译单元链接及多源文件 CMake 构建验证。
- 使用 CMake `FetchContent` 固定获取 GLFW 3.4，并成功构建、链接其静态库。
- 使用 GLFW 创建 800×600 窗口，完成 OpenGL context 激活、垂直同步、事件循环和资源清理，并验证窗口可正常关闭。
- 添加 `.clang-format` 和 VS Code 工作区设置，统一 C/C++ 格式并启用保存时自动格式化。
- 添加 `GlfwRuntime` 和 `Window` RAII 封装，使 GLFW 全局环境与窗口句柄按对象作用域自动释放。
- 配置 OpenGL 3.3 Core Profile context，使用跨平台 CMake 目标 `OpenGL::GL` 链接 OpenGL，并完成每帧颜色缓冲清除。
- 使用 framebuffer 实际像素尺寸初始化 OpenGL viewport，并在 framebuffer 尺寸变化时通过 GLFW callback 同步更新。
- 集成 GLAD 2.0.8 的 OpenGL 3.3 Core 函数加载代码，在 context 激活后通过 `glfwGetProcAddress` 加载现代 OpenGL API。
- 建立三角形顶点数据、VAO 和 VBO，配置顶点属性读取规则。
- 编写并编译最小顶点着色器和片段着色器，加入编译状态与错误日志检查。
- 创建、链接并启用 Shader Program，加入链接状态与错误日志检查。
- 使用 `glDrawArrays` 完成第一个橙色三角形的渲染，并验证运行画面。
- 实现 `ShaderProgram` 的基础 RAII 接口：析构时释放 Program、`use()` 启用 Program、
  `isValid()` 判断资源有效性，并将源文件加入 CMake 构建。
- 将单个 Shader 的创建、源码设置、编译状态检查和错误日志封装为文件内辅助函数，
  并保证编译失败时释放临时 Shader 资源。
- 在 `ShaderProgram` 构造函数中接入顶点和片段 Shader 编译，并覆盖单边失败与双边成功时的
  临时资源清理路径。
- 完成 `ShaderProgram` 的 Program 创建、Shader 附加、链接状态与错误日志检查；链接失败时
  释放 Program 并恢复无效状态，链接成功后由析构函数管理其生命周期。
- 使用 `ShaderProgram` 替换 `App::run` 中手动编译 Shader、链接 Program 和释放 Program 的
  原始流程，并通过完整构建与运行画面验证，橙色三角形保持正常显示。

## 进行中

- 开始引入 GLM，先建立模型变换与 Shader uniform 的最小工作流。

## 关键决策

- 使用 C++20、VS Code、CMake 和 Ninja。
- macOS 使用 Apple Clang，Windows 使用 MSVC Build Tools。
- 不使用游戏引擎。
- 图形使用 OpenGL、GLFW 和 GLM。
- 基础渲染 context 使用 OpenGL 3.3 Core Profile；macOS 启用 forward compatibility，并显式接受其 OpenGL 弃用状态。
- GLFW 通过 CMake `FetchContent` 获取并固定为 3.4，避免依赖每台设备手动安装。
- GLAD 生成代码直接纳入仓库并构建为独立静态库，避免在各平台配置阶段引入 Python 生成依赖。
- C/C++ 代码基于 LLVM 风格使用 4 空格缩进、100 列限制和左侧指针对齐，并由 VS Code 保存时自动格式化。
- GLFW 资源采用 RAII 管理；禁止复制拥有资源的对象，并通过局部对象的逆序析构保证窗口先于 GLFW 全局环境释放。
- Shader 和 Program 采用不同生命周期：Shader 在 Program 链接成功后即可删除，Program
  保留到渲染结束；`ShaderProgram` RAII 类负责完整管理 Program。
- 网络阶段优先使用原生 UDP Socket。
- 服务器拥有最终权威。
- 开发过程采用“有脚手架的亲手实现”：先讲最小必要概念，再由用户尝试核心实现，卡住时分级提示，之后共同审查和复盘。

## 下一步

1. 通过 CMake 引入 GLM，并验证跨平台目标链接。
2. 为 `ShaderProgram` 增加最小的 `mat4` uniform 设置接口。
3. 使用模型矩阵平移三角形，验证 CPU 到 Shader 的矩阵数据传递。

## 维护规则

- 只记录可持续复用的上下文，不复制完整对话。
- 完成阶段目标后更新“当前阶段”和“已完成”。
- 出现影响后续实现的设计选择时更新“关键决策”。
- “下一步”只保留当前最相关的少量任务。
