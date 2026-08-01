# 项目状态

最后更新：2026-08-01

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
- `Window` 在不暴露 GLFW 句柄的前提下提供光标捕获与位置查询。
- 鼠标或触摸板移动可以通过 yaw 和 pitch 改变摄像机朝向，pitch 限制在
  `[-89°, 89°]` 以避免视角翻转。
- 已从 `App::run()` 提取独立的 `Camera` 类，集中管理摄像机状态、移动、旋转与观察矩阵；
  `Camera` 不依赖 GLFW、`Window` 或 OpenGL。
- 按 `Esc` 可以释放光标，点击鼠标左键可以重新捕获光标。
- 光标释放期间摄像机移动与旋转暂停，但事件处理和渲染保持运行；重新捕获时会重置
  鼠标位置基准，避免视角跳变。
- Cursor/VS Code 可以通过 CodeLLDB 启动并调试 `fps_client`。
- GLFW 3.4 与 GLM 1.0.3 已通过 CMake `FetchContent` 接入并固定版本。
- 已启用深度测试，并在清屏时同时清理颜色缓冲和深度缓冲，物体前后遮挡关系正确。
- 可以复用同一份立方体 VAO/VBO，通过逐物体上传独立的模型矩阵绘制分布在不同位置和
  深度的简单 3D 场景。
- `Window` 可以在不暴露 GLFW 句柄的前提下查询当前 framebuffer 尺寸；投影矩阵会使用
  实际宽高比逐帧更新，窗口缩放时场景比例保持正确，并安全处理零尺寸 framebuffer。
- `ShaderProgram` 支持按名称上传 `vec3` uniform；场景可在每次绘制前设置独立纯色。
- 已复用立方体网格，通过平移和非均匀缩放绘制大型地面平台；地面与立方体颜色可区分，
  并能提供稳定的高度、方向和移动速度参照。

## 当前任务

评估并实现 Phase 1 的简单碰撞边界。

验收标准：

- 明确玩家碰撞体、场景碰撞体以及碰撞检测所属职责，不把碰撞逻辑放进渲染代码。
- 摄像机移动到选定的简单碰撞边界时无法继续穿入，但可以正常离开和沿边界移动。
- 碰撞结果不依赖帧率，并能通过少量独立数据或函数验证。

## 后续任务

1. 加入射线射击与简单命中检测。

## 当前实现决策

- 使用 OpenGL 3.3 Core Profile；macOS 启用 forward compatibility。
- GLAD 生成代码直接纳入仓库并构建为独立静态库。
- GLFW 和 GLM 通过 `FetchContent` 获取并固定版本。
- 拥有 OpenGL 或 GLFW 资源的对象采用 RAII，并禁止复制。
- `Camera` 只负责摄像机状态和数学运算，不直接读取输入或调用 OpenGL。
- Shader 链接完成后释放；Program 由 `ShaderProgram` 持有到使用结束。
- 简单场景中的多个物体共享立方体网格，通过各自的模型矩阵和颜色 uniform 区分。
- 共享箱体网格的局部坐标统一为三个轴上的 `[-0.5, 0.5]`；`Box` 使用 `center`、`size`
  和 `color` 描述实例，使渲染变换与后续 AABB 边界可以共用同一份空间数据。
- MSVC 编译 `fps_client` 时显式使用 `/utf-8`，与仓库 UTF-8 文本约定保持一致。
- C/C++ 格式基于 LLVM 风格：4 空格缩进、100 列限制、左侧指针对齐。

## 已知问题或阻塞

- 暂无。
