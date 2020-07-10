#include "program.h"

namespace sid {
Program::Program() { m_ProgramId = glCreateProgram(); }

void Program::AddShader(ShaderPtr shader) { m_Shaders.emplace_back(std::move(shader)); }

bool Program::Compile() {
    if (m_IsCompiled) {
        return true;
    }

    for (auto &shader : m_Shaders) {
        auto shader_comp = shader->Compile();
        if (!shader_comp) {
            return false;
        }

        glAttachShader(m_ProgramId, shader->GetId());
    }

    glLinkProgram(m_ProgramId);

    m_IsCompiled = true;
    return true;
}

void Program::Apply() const { glUseProgram(m_ProgramId); }

// Maps to glUniformXX with id storage and caching
void Program::SetUniform1f(const char *attrib_name, GLfloat v0) {
    auto location = GetLocation(attrib_name);

    glUniform1f(location, v0);
}

void Program::SetUniform2f(const char *attrib_name, GLfloat v0, GLfloat v1) {
    auto location = GetLocation(attrib_name);

    glUniform2f(location, v0, v1);
}

void Program::SetUniform3f(const char *attrib_name, GLfloat v0, GLfloat v1, GLfloat v2) {
    auto location = GetLocation(attrib_name);

    glUniform3f(location, v0, v1, v2);
}
void Program::SetUniform4f(const char *attrib_name, GLfloat v0, GLfloat v1, GLfloat v2,
                           GLfloat v3) {
    auto location = GetLocation(attrib_name);

    glUniform4f(location, v0, v1, v2, v3);
}

void Program::SetUniform1i(const char *attrib_name, GLint v0) {
    auto location = GetLocation(attrib_name);

    glUniform1i(location, v0);
}

void Program::SetlUniform2i(const char *attrib_name, GLint v0, GLint v1) {
    auto location = GetLocation(attrib_name);

    glUniform2i(location, v0, v1);
}

void Program::SetUniform3i(const char *attrib_name, GLint v0, GLint v1, GLint v2) {
    auto location = GetLocation(attrib_name);

    glUniform3i(location, v0, v1, v2);
}

void Program::SetUniform4i(const char *attrib_name, GLint v0, GLint v1, GLint v2,
                           GLint v3) {
    auto location = GetLocation(attrib_name);

    glUniform4i(location, v0, v1, v2, v3);
}

void Program::SetUniform1ui(const char *attrib_name, GLuint v0) {
    auto location = GetLocation(attrib_name);

    glUniform1ui(location, v0);
}

void Program::SetUniform2ui(const char *attrib_name, GLuint v0, GLuint v1) {
    auto location = GetLocation(attrib_name);

    glUniform2ui(location, v0, v1);
}

void Program::SetUniform3ui(const char *attrib_name, GLuint v0, GLuint v1, GLuint v2) {
    auto location = GetLocation(attrib_name);

    glUniform3ui(location, v0, v1, v2);
}

void Program::SetUniform4ui(const char *attrib_name, GLuint v0, GLuint v1, GLuint v2,
                            GLuint v3) {
    auto location = GetLocation(attrib_name);

    glUniform4ui(location, v0, v1, v2, v3);
}

void Program::SetUniformArray(const char *attrib_name, Array type, GLsizei count,
                              void_ptr value) {
    auto location = GetLocation(attrib_name);
    switch (type) {
        case Array::fv1:
            glUniform1fv(location, count, value.c_ptr<GLfloat>());
            break;
        case Array::fv2:
            glUniform2fv(location, count, value.c_ptr<GLfloat>());
            break;
        case Array::fv3:
            glUniform3fv(location, count, value.c_ptr<GLfloat>());
            break;
        case Array::fv4:
            glUniform4fv(location, count, value.c_ptr<GLfloat>());
            break;
        case Array::iv1:
            glUniform1iv(location, count, value.c_ptr<GLint>());
            break;
        case Array::iv2:
            glUniform2iv(location, count, value.c_ptr<GLint>());
            break;
        case Array::iv3:
            glUniform3iv(location, count, value.c_ptr<GLint>());
            break;
        case Array::iv4:
            glUniform4iv(location, count, value.c_ptr<GLint>());
            break;
        case Array::uiv1:
            glUniform1uiv(location, count, value.c_ptr<GLuint>());
            break;
        case Array::uiv2:
            glUniform2uiv(location, count, value.c_ptr<GLuint>());
            break;
        case Array::uiv3:
            glUniform3uiv(location, count, value.c_ptr<GLuint>());
            break;
        case Array::uiv4:
            glUniform4uiv(location, count, value.c_ptr<GLuint>());
            break;
        default:
            break;
    }
}

void Program::SetUniformMatrix(const char *attrib_name, Matrix type, GLsizei count,
                               GLboolean transpose, const GLfloat *value) {
    auto location = GetLocation(attrib_name);

    switch (type) {
        case Matrix::fv2:
            glUniformMatrix2fv(location, count, transpose, value);
            break;
        case Matrix::fv3:
            glUniformMatrix3fv(location, count, transpose, value);
            break;
        case Matrix::fv4:
            glUniformMatrix4fv(location, count, transpose, value);
            break;
        case Matrix::fv2x3:
            glUniformMatrix2x3fv(location, count, transpose, value);
            break;
        case Matrix::fv3x2:
            glUniformMatrix3x2fv(location, count, transpose, value);
            break;
        case Matrix::fv2x4:
            glUniformMatrix2x4fv(location, count, transpose, value);
            break;
        case Matrix::fv4x2:
            glUniformMatrix4x2fv(location, count, transpose, value);
            break;
        case Matrix::fv3x4:
            glUniformMatrix3x4fv(location, count, transpose, value);
            break;
        case Matrix::fv4x3:
            glUniformMatrix4x3fv(location, count, transpose, value);
            break;
    }
}

GLint Program::GetLocation(const char *attrib_name) {
    auto find_it = m_Locations.find(attrib_name);
    if (find_it != std::end(m_Locations)) {
        return find_it->second;
    }

    auto location = glGetUniformLocation(m_ProgramId, attrib_name);
    m_Locations[attrib_name] = location;

    return location;
}

}  // namespace sid