# FPSdemo 项目协作说明

## 项目目标

构建一个不依赖游戏引擎的多人 FPS 网络同步 Demo。重点是理解现代多人游戏网络架构，而不是快速完成一个完整游戏。

核心机制：

- Client / Server 架构
- Server authoritative
- Tick-based simulation
- Input synchronization
- Client-side prediction
- Server reconciliation
- Interpolation
- Lag compensation / server rewind

## 技术约束

- C++20
- VS Code
- CMake + Ninja
- Windows：MSVC（Visual Studio Build Tools，不使用 Visual Studio IDE）
- macOS：Apple Clang
- 图形：OpenGL + GLFW + GLM
- 网络：优先使用原生 UDP Socket，之后再考虑 ENet 或 Boost.Asio
- 不使用游戏引擎

## 开发阶段

1. 单机基础：窗口、FPS 摄像机、WASD、鼠标视角、简单碰撞、射线射击
2. 基础同步：客户端发送输入，服务器更新权威状态并发送快照
3. 客户端预测：本地立即模拟输入
4. 服务器校正：输入历史、回滚与重放
5. 高级同步：插值、延迟补偿与服务器回溯

## Agent 协作规则

- 开始任务前先阅读 `docs/PROJECT_STATUS.md`，确保工作与当前进度一致。
- 完成会改变项目进度、关键决策或下一步计划的工作后，更新 `docs/PROJECT_STATUS.md`。
- 默认以导师方式协作，不一次性生成完整项目。
- 只回答用户当前提出的具体问题，除非用户明确要求扩展。
- 编写代码前先简要说明设计意图和关键取舍。
- 遇到问题时优先帮助用户分析错误、提出验证步骤和调试方向。
- 未经明确要求，不替用户大规模重写代码或提前实现后续阶段。
- 服务器拥有最终权威；客户端负责输入、渲染、本地预测和平滑显示。
- 每次改动保持小而可验证，并遵循项目当前阶段。
- 跨平台代码应同时考虑 Windows/MSVC 与 macOS/Apple Clang。
- 使用 Python 时优先使用 `uv`，但尊重仓库已有工作流。
- 本学习项目在 macOS 上的相关笔记保存在 `/Users/tangcheng/笔记/SyncedLib/CodeNote/FPSdemo`；仅在用户明确要求时记录，使用 Obsidian 兼容的 Markdown，内容保持简洁清晰、合理分条，并按需使用内部链接、引用块和代码引用。
- Windows 与 macOS 使用同步的同一笔记库；在 Windows 上记录笔记时，应使用该设备对应的同步笔记库路径，不使用 macOS 绝对路径。
