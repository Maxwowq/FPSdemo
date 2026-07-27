#pragma once

class GlfwRuntime {
  public:
    GlfwRuntime();
    ~GlfwRuntime();

    GlfwRuntime(const GlfwRuntime&) = delete;
    GlfwRuntime& operator=(const GlfwRuntime&) = delete;

    bool isInitialized() const;

  private:
    bool initialized_ = false;
};
