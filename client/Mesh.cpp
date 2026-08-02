#include "Mesh.h"

Mesh::Mesh(std::span<const float> vertices, const GLenum mode)
    : mode_(mode), vertexCount_(vertices.size() / 3) {
    // 生成VAO和VBO
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    // 绑定VAO和VBO
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    // 复制数据到vbo
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(),
                 GL_STATIC_DRAW);
    // 将读取规则记录到vao
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    // 启动location:0顶点属性数组
    glEnableVertexAttribArray(0);

    // 解绑
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

// 绘图
void Mesh::drawArrays() const {
    glBindVertexArray(vao_);
    glDrawArrays(mode_, 0, vertexCount_);
}
