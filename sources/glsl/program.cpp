#include "program.h"

namespace sid {
Program::Program() { m_ProgramId = glCreateProgram(); }

void Program::AddShader(ShaderPtr shader) { m_Shaders.emplace_back(std::move(shader)); }

bool Program::Compile() {
    if (m_IsCompiled) {
        return true;
    }

    for (auto& shader : m_Shaders) {
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
}  // namespace sid