# 项目状态

最后更新：2026-07-28

## 当前阶段

Phase 1：单机 FPS 基础

当前目标：搭建开发环境和项目结构，完成第一个最小可运行程序。

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

## 关键决策

- 使用 C++20、VS Code、CMake 和 Ninja。
- macOS 使用 Apple Clang，Windows 使用 MSVC Build Tools。
- 不使用游戏引擎。
- 图形使用 OpenGL、GLFW 和 GLM。
- GLFW 通过 CMake `FetchContent` 获取并固定为 3.4，避免依赖每台设备手动安装。
- C/C++ 代码基于 LLVM 风格使用 4 空格缩进、100 列限制和左侧指针对齐，并由 VS Code 保存时自动格式化。
- GLFW 资源采用 RAII 管理；禁止复制拥有资源的对象，并通过局部对象的逆序析构保证窗口先于 GLFW 全局环境释放。
- 网络阶段优先使用原生 UDP Socket。
- 服务器拥有最终权威。
- 开发过程采用“有脚手架的亲手实现”：先讲最小必要概念，再由用户尝试核心实现，卡住时分级提示，之后共同审查和复盘。

## 下一步

1. 配置明确的 OpenGL context 版本并建立基础渲染流程。
2. 加入窗口尺寸变化处理和第一帧清屏。
3. 确定现代 OpenGL 函数加载方式。

## 维护规则

- 只记录可持续复用的上下文，不复制完整对话。
- 完成阶段目标后更新“当前阶段”和“已完成”。
- 出现影响后续实现的设计选择时更新“关键决策”。
- “下一步”只保留当前最相关的少量任务。
