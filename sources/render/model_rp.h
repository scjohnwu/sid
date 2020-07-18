#include "render_pass.h"

// opengl stuff
#include "utility/opengl_support.h"

// scene components
#include "scene/model.h"

namespace sid {
class ModelRenderPass : public RenderPass {
   public:
    ~ModelRenderPass();

    void Init();

    void Draw() override;

    void SetModel(ModelPtr model);
   private:
    void InitGeometry();
    void InitShader();

    VertexArrayPtr m_VAO;
    BufferPtr m_VBO;
    BufferPtr m_EBO;

    VertexAttribPtr m_Position;

    ModelPtr m_RenderModel;

    ProgramPtr m_Program;
    
    ShaderPtr m_VertexShader;
    ShaderSourcePtr m_VertexShaderSource;

    ShaderPtr m_FragmentShader;
    ShaderSourcePtr m_FragmentShaderSource;
};
using ModelRenderPassPtr = std::shared_ptr<ModelRenderPass>;

}  // namespace sid