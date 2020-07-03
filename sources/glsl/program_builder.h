#pragma once

#include "program.h"

namespace sid {
class ProgramBuilder {
   public:
    ProgramBuilder& NewProgram();
    ProgramBuilder& AddShader(std::string file_name, ShaderType);
    ProgramPtr Build();

   private:
    ProgramPtr m_CurrentProgram{nullptr};
};
}  // namespace sid