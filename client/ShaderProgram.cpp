#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

// 匿名命名空间其他cpp不可见
namespace {
// 内部shader处理函数
GLuint makeShader(const char* source, GLenum type) {
    // 创建shader
    const GLuint shader = glCreateShader(type);
    // 存入源码
    glShaderSource(shader, 1, &source, nullptr);
    // 编译
    glCompileShader(shader);

    // 检查编译结果是否正常
    GLint compileStatus = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        // 查询错误日志字符空间长度
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        // 分配缓存空间
        std::string logBuffer(static_cast<std::size_t>(logLength), '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, logBuffer.data());
        // 输出日志
        std::string shaderType("Unknown");
        if (type == GL_VERTEX_SHADER) {
            shaderType = "Vertex";
        } else if (type == GL_FRAGMENT_SHADER) {
            shaderType = "Fragment";
        }

        std::cerr << shaderType << " Shader Compilation Failed:\n" << logBuffer;
        // 释放shader
        glDeleteShader(shader);
        // 结束运行，返回表示编译失败
        return 0;
    }

    return shader;
}
} // namespace

// 构造函数
ShaderProgram::ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // 先编译vertexShader
    GLuint vertexShader = makeShader(vertexShaderSource, GL_VERTEX_SHADER);
    // 若失败，直接退出
    if (vertexShader == 0) {
        return;
    }
    // 再编译fragmentShader
    GLuint fragmentShader = makeShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    // 若失败，清除vertexShader然后退出
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return;
    }

    // 创建program并存入成员变量
    program = glCreateProgram();
    // 若创建失败，直接释放shader并结束
    if (program == 0) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }
    // 装入shader
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    // 链接program
    glLinkProgram(program);

    // 无论成功与否，都释放shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 查询program链接状态
    GLint programStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &programStatus);
    // 若链接失败
    if (programStatus != GL_TRUE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::string logBuffer(static_cast<size_t>(logLength), '\0');
        glGetProgramInfoLog(program, logLength, nullptr, logBuffer.data());
        std::cerr << "Shader Program Link Failed:\n" << logBuffer;
        // 释放program
        glDeleteProgram(program);
        // 恢复成员变量为0
        program = 0;
    }
}

// 析构函数，释放program
ShaderProgram::~ShaderProgram() {
    if (isValid()) {
        glDeleteProgram(program);
    }
}

// 启用program
void ShaderProgram::use() const {
    glUseProgram(program);
}

bool ShaderProgram::isValid() const {
    return program != 0;
}

void ShaderProgram::setMat4(const char* name, const glm::mat4& value) const {
    // 查找uniform地址
    const GLint location = glGetUniformLocation(program, name);
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " location not found";
        return;
    }

    // 上传矩阵
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setVec3(const char* name, const glm::vec3& value) const {
    // 查找uniform地址
    const GLint location = glGetUniformLocation(program, name);
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " location not found";
        return;
    }

    // 上传向量
    glUniform3fv(location, 1, glm::value_ptr(value));
}
