#include <memory>
#include <string>

#include "utility/opengl_support.h"

namespace sid {
class Model {
   public:
    void Draw();
    void LoadData(std::string filename);

    VertexArrayPtr m_VAO;
    BufferPtr m_VBO;
    BufferPtr m_EBO;

    VertexAttribPtr m_Position;

    ProgramPtr m_Program;

    ShaderPtr m_VertexShader;
    ShaderSourcePtr m_VertexShaderSource;

    ShaderPtr m_FragmentShader;
    ShaderSourcePtr m_FragmentShaderSource;

    std::size_t m_Indexes{0};
};
using ModelPtr = std::shared_ptr<Model>;

ModelPtr make_model();
}  // namespace sid