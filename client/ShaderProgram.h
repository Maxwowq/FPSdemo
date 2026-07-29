#pragma once
#include <glad/gl.h>
#include <glm/mat4x4.hpp>

class ShaderProgram {
  public:
    ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~ShaderProgram();

    void use() const;

    bool isValid() const;

    void setMat4(const char* name, const glm::mat4& value) const;

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

  private:
    GLuint program = 0;
};
