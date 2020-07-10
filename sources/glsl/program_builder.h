#pragma once

#include "program.h"

namespace sid {
class ProgramBuilder {
   public:
    ProgramBuilder& NewProgram();
    ProgramBuilder& AddShader(std::string file_name, ShaderType);
    ProgramBuilder& AddShaderSrc(std::string src, ShaderType );
    ProgramPtr Build();

   private:
    ProgramPtr m_CurrentProgram{nullptr};
};
}  // namespace sid