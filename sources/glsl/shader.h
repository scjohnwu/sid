#pragma once

#include <memory>
#include <string>

#include "utility/opengl_support.h"

namespace sid {
enum class ShaderType {
    Vertex,
    Geometry,
    Fragment,
    Tesselation_Eval,
    Tesselation_Control
};
GLenum shader_type_to_enum(ShaderType type);

class Shader {
   public:
    Shader(std::string source_code, ShaderType shader_type);

    bool Compile();

    GLuint GetId() const;
    ShaderType GetType() const;

   private:
    std::string m_SourceCode;

    ShaderType m_ShaderType;

    GLuint m_ShaderId{0};

    bool m_IsCompiled{false};
};
using ShaderPtr = std::unique_ptr<Shader>;
}  // namespace sid