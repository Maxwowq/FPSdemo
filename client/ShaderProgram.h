#pragma once
#include <glad/gl.h>

class ShaderProgram {
  public:
    ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~ShaderProgram();

    void use() const;

    bool isValid() const;

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

  private:
    GLuint program = 0;
};
