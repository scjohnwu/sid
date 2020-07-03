#pragma once

#include <vector>

#include "shader.h"

namespace sid {
class Program {
   public:
    Program();
    void AddShader(ShaderPtr shader);
    bool Compile();
    void Apply() const;

   private:
    std::vector<ShaderPtr> m_Shaders;
    GLuint m_ProgramId{0};
    bool m_IsCompiled{false};
};
}  // namespace sid