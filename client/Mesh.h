#pragma once
#include <glad/gl.h>
#include <span>

class Mesh {
  public:
    Mesh(std::span<const float> vertices, const GLenum mode);
    ~Mesh();

    void drawArrays() const;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

  private:
    GLuint vao_;
    GLuint vbo_;

    const GLenum mode_;

    const GLsizei vertexCount_;
};
