# 项目状态

最后更新：2026-08-02

## 当前阶段

Phase 1：单机 FPS 基础

## 当前能力

- 可以创建 OpenGL 3.3 Core Profile 窗口，通过 GLAD 使用现代 OpenGL，并正确响应
  framebuffer 尺寸变化。
- `GlfwRuntime`、`Window` 和 `ShaderProgram` 使用 RAII 管理资源生命周期；`Window`
  隐藏 GLFW 句柄并提供输入、光标和 framebuffer 查询接口。
- `ShaderProgram` 支持 Shader 编译链接、错误日志，以及按名称上传 `mat4` 和 `vec3`
  uniform；场景使用完整 MVP 变换与逐物体纯色渲染。
- `Camera` 独立管理位置、朝向和观察矩阵，不依赖 GLFW、`Window` 或 OpenGL；鼠标通过
  yaw/pitch 控制视角，pitch 限制在 `[-89°, 89°]`。
- W/A/S/D 提供帧率无关的水平移动；组合方向会归一化，斜向移动不会加速。
- 玩家和场景箱体使用 AABB 进行水平碰撞检测；碰撞时阻止继续穿入，同时允许离开障碍物
  和沿障碍物表面滑动，并通过位移子步降低高速移动穿透的风险。
- 按 `Esc` 可释放光标，点击左键可重新捕获；暂停控制和重新捕获不会造成视角跳变。
- 场景通过可复用的 `Mesh` 绘制中心化单位箱体及其黑色边线；多个彩色箱体和大型地面
  平台共用网格资源，深度遮挡正确，并能提供位置、距离、高度和移动速度参照。
- 投影矩阵使用实际 framebuffer 宽高比逐帧更新，窗口缩放时场景比例保持正确，并安全
  处理零尺寸 framebuffer。
- 项目可通过 CMake、Ninja 和 MSVC Build Tools 构建，并可在 VS Code 中使用 CodeLLDB
  启动和调试 `fps_client`。

## 当前任务

加入射线射击与简单命中检测。

验收标准：

- 从摄像机位置和朝向生成射线。
- 射线能够与场景箱体求交，并选出射线方向上距离最近的命中目标。
- 射击输入只触发一次检测，命中结果可以通过简单可见反馈验证。

## 后续任务

1. 在射击命中稳定后，确定 Phase 1 下一项最小任务。

## 当前实现决策

- 使用 OpenGL 3.3 Core Profile；macOS 启用 forward compatibility。
- GLAD 生成代码直接纳入仓库并构建为独立静态库。
- GLFW 3.4 和 GLM 1.0.3 通过 `FetchContent` 获取并固定版本。
- 拥有 OpenGL 或 GLFW 资源的对象采用 RAII，并禁止复制。
- 输入逻辑生成期望位移，碰撞逻辑修正位移，`Camera` 只管理观察状态并应用最终位移；
  `Camera` 不直接读取输入、处理碰撞或调用 OpenGL。
- 玩家使用相对摄像机位置固定尺寸的 AABB；场景箱体的 AABB 由 `center` 和 `size`
  计算。水平位移按 X、Z 轴依次检测以支持贴边滑动，并拆成最大长度约为 `0.1` 的子步。
- Shader 链接完成后释放；Program 由 `ShaderProgram` 持有到使用结束。
- 共享箱体网格的局部坐标统一为三个轴上的 `[-0.5, 0.5]`；`Box` 使用 `center`、`size`
  和 `color` 描述实例，使渲染变换与后续 AABB 边界可以共用同一份空间数据。
- `Mesh` 使用 RAII 管理 VAO/VBO，保存绘制模式和顶点数，并禁止复制；箱体三角形与边线
  分别使用一个共享 `Mesh`。
- MSVC 编译 `fps_client` 时显式使用 `/utf-8`，与仓库 UTF-8 文本约定保持一致。
- C/C++ 格式基于 LLVM 风格：4 空格缩进、100 列限制、左侧指针对齐。

## 已知问题或阻塞

- 暂无。
