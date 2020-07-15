#include "render_pass.h"

// opengl stuff
#include "utility./opengl_support.h"

namespace sid {
class SimpleRenderPass : public RenderPass {
   public:
    void Init();

    void Draw() override;

   private:
    void InitGeometry();
    void InitShader();

    VertexArrayPtr m_VAO;
    BufferPtr m_VBO;
    BufferPtr m_EBO;

    VertexAttribPtr m_Position;

    ProgramPtr m_Program;
    
    ShaderPtr m_VertexShader;
    ShaderSourcePtr m_VertexShaderSource;

    ShaderPtr m_FragmentShader;
    ShaderSourcePtr m_FragmentShaderSource;
};
using SimpleRenderPassPtr = std::shared_ptr<SimpleRenderPass>;

}  // namespace sid