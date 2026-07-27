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

  private:
    GLFWwindow* handle_ = nullptr;
};
