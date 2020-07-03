#include "shader.h"

namespace sid {
GLenum shader_type_to_enum(ShaderType shader_type) {
    switch (shader_type) {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderType::Tesselation_Control:
            return GL_TESS_CONTROL_SHADER;
        case ShaderType::Tesselation_Eval:
            return GL_TESS_EVALUATION_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
    }
    return 0;
}

Shader::Shader(std::string source_code, ShaderType shader_type)
    : m_SourceCode{source_code}, m_ShaderType{shader_type} {
    m_ShaderId = glCreateShader(shader_type_to_enum(shader_type));
}

bool Shader::Compile() {
    if (m_IsCompiled) {
        return true;
    }

    auto src = m_SourceCode.c_str();
    const GLchar** lp_source = &src;

    glShaderSource(m_ShaderId, 1, lp_source, nullptr);
    glCompileShader(m_ShaderId);

    m_IsCompiled = true;
    // TODO:: return error checks
    return true;
}

GLuint Shader::GetId() const { return m_ShaderId; }

ShaderType Shader::GetType() const { return m_ShaderType; }

}  // namespace sid