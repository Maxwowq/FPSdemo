# FPSdemo 项目协作说明

## 项目目标

构建一个不依赖游戏引擎的多人 FPS 网络同步 Demo。目标是逐步理解现代多人游戏的网络架构，
而不是快速完成一款完整游戏。

核心机制包括 Client / Server、服务器权威、固定 Tick、输入同步、客户端预测与协调、
远端玩家插值，以及延迟补偿与服务器回溯。

## 技术约束

- C++20、VS Code、CMake + Ninja，不使用游戏引擎。
- Windows 使用 MSVC Build Tools；macOS 使用 Apple Clang。
- 图形使用 OpenGL、GLFW 和 GLM。
- 网络阶段优先使用原生 UDP Socket，之后再按需要评估 ENet 或 Boost.Asio。
- 跨平台代码必须同时考虑 Windows/MSVC 与 macOS/Apple Clang。
- 使用 Python 时优先使用 `uv`，但尊重仓库已有工作流。

## Windows 环境约定

- 仓库文本文件统一按 UTF-8 读取和写入。Windows PowerShell 5.1 的默认代码页可能是
  GBK；读取中文文件时必须显式指定 UTF-8，并在需要展示中文输出时将控制台输出编码和
  `$OutputEncoding` 设置为 UTF-8，不能根据乱码内容判断文件已损坏。
- 当前终端找不到 `cmake`、`ninja` 或 MSVC 时，不应直接判定环境无法构建。优先检查已有
  `build/CMakeCache.txt`，使用其中记录的 `CMAKE_COMMAND`、`CMAKE_MAKE_PROGRAM` 和
  编译器绝对路径。Windows 上真正重新编译前必须先调用当前 Visual Studio Build Tools 的
  `VsDevCmd.bat`（x64），确保 MSVC 标准库和 Windows SDK 环境已加载；然后通过缓存所记录的
  CMake 执行 `cmake --build build --target fps_client`。仅在 Ninja 报告无工作时，未加载开发
  环境的构建命令才可能表面成功。
- 只有在缓存不存在、记录的工具路径失效，或实际构建命令失败后，才将工具链视为未配置，
  并清楚报告具体缺失项或错误信息。

## 开发路线

1. 单机基础：窗口、FPS 摄像机、输入、简单碰撞与射线射击。
2. 权威同步：客户端发送输入，服务器按固定 Tick 模拟并发送状态快照。
3. 客户端预测：本地立即模拟自己的输入，减少操作延迟。
4. 客户端协调：依据服务器确认的输入序号恢复权威状态，并重放未确认输入。
5. 高级同步：远端实体插值、延迟补偿与服务器回溯。

服务器始终拥有最终权威；客户端只负责采集输入、渲染、本地预测与平滑显示。

## 协作方式

- 开始任务前阅读 `docs/PROJECT_STATUS.md`，只处理当前阶段和用户明确提出的问题。
- 核心学习代码优先由用户亲手实现。Agent 先说明最小必要概念、结构边界和验收标准，
  用户卡住时再依次提供思路、伪代码、局部示例和完整实现。
- 不要求用户凭空设计尚未学习的架构；机械配置、重复劳动和工具兼容问题可由 Agent 直接处理。
- 编写代码前简要说明设计意图和关键取舍；代码审查先解释正确性、职责划分和取舍。
- 改动保持小而可验证，不大规模重写，也不提前实现后续阶段。
- 每一步应可编译、可运行或可独立验证。

## 任务收尾

- 工作改变当前能力、实现决策或后续任务时，更新 `docs/PROJECT_STATUS.md`。
- 完成可独立验收的开发板块后，确认构建或运行结果、检查 Git 变更并提醒用户提交。
- 未经用户明确要求，不代替用户创建 Git 提交。

## 学习笔记

- 仅在用户明确要求时记录笔记，使用简洁的 Obsidian 兼容 Markdown。
- macOS 笔记目录为 `/Users/tangcheng/笔记/SyncedLib/CodeNote/FPSdemo`。
- Windows 使用同一同步笔记库在该设备上的实际路径，不引用 macOS 绝对路径。
