#pragma once

struct GLFWwindow;

class Window {
  public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool isValid() const;
    void makeContextCurrent();
    bool shouldClose() const;
    void swapBuffers();

    void updateViewport();

    bool isKeyPressed(int key) const;

    void setCursorDisabled();
    void setCursorNormal();
    void getCursorPos(double& xPosition, double& yPosition) const;

  private:
    GLFWwindow* handle_ = nullptr;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};
