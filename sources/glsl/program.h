#pragma once

#include <unordered_map>
#include <vector>

#include "shader.h"

namespace sid {
struct void_ptr {
    void_ptr(const void *ptr) : m_Ptr{ptr} {};

    void_ptr(const void_ptr &) = delete;
    void_ptr(void_ptr &&) = delete;

    void_ptr &operator=(const void_ptr &) = delete;
    void_ptr &operator=(void_ptr &&) = delete;

    const void *m_Ptr{nullptr};

    template <typename T>
    const T *c_ptr() {
        return reinterpret_cast<const T *>(m_Ptr);
    }

    template <typename T>
    T *ptr() {
        return reinterpret_cast<T *>(m_Ptr);
    }
};

template <typename T>
void_ptr to_void(const T *ptr) {
    return void_ptr{reinterpret_cast<const void *>(ptr)};
}

class Program {
   public:
    Program();
    void AddShader(ShaderPtr shader);
    bool Compile();
    void Apply() const;

    // Maps to glUniformXX with id storage and caching
    void SetUniform1f(const char *location_name, GLfloat v0);
    void SetUniform2f(const char *location_name, GLfloat v0, GLfloat v1);
    void SetUniform3f(const char *location_name, GLfloat v0, GLfloat v1, GLfloat v2);
    void SetUniform4f(const char *location_name, GLfloat v0, GLfloat v1, GLfloat v2,
                      GLfloat v3);
    void SetUniform1i(const char *location_name, GLint v0);
    void SetlUniform2i(const char *location_name, GLint v0, GLint v1);
    void SetUniform3i(const char *location_name, GLint v0, GLint v1, GLint v2);
    void SetUniform4i(const char *location_name, GLint v0, GLint v1, GLint v2, GLint v3);
    void SetUniform1ui(const char *location_name, GLuint v0);
    void SetUniform2ui(const char *location_name, GLuint v0, GLuint v1);
    void SetUniform3ui(const char *location_name, GLuint v0, GLuint v1, GLuint v2);
    void SetUniform4ui(const char *location_name, GLuint v0, GLuint v1, GLuint v2,
                       GLuint v3);

    // Maps to glUniformXXfv
    enum class Array { fv1, fv2, fv3, fv4, iv1, iv2, iv3, iv4, uiv1, uiv2, uiv3, uiv4 };
    void SetUniformArray(const char *location_name, Array type, GLsizei count,
                         void_ptr value);

    // Maps to glUniformMatrixXXfv
    enum class Matrix { fv2, fv3, fv4, fv2x3, fv3x2, fv2x4, fv4x2, fv3x4, fv4x3 };
    void SetUniformMatrix(const char *location_name, Matrix type, GLsizei count,
                          GLboolean transpose, const GLfloat *value);

   private:
    std::vector<ShaderPtr> m_Shaders;
    std::unordered_map<const char *, GLint> m_Locations;

    GLint GetLocation(const char *location_name);

    GLuint m_ProgramId{0};
    bool m_IsCompiled{false};
};
using ProgramPtr = std::shared_ptr<Program>;
}  // namespace sid