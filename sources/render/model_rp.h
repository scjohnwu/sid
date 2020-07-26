#include "render_pass.h"

// opengl stuff
#include "utility/opengl_support.h"

// scene components
#include "scene/camera.h"
#include "scene/scene.h"

namespace sid {
class ModelRenderPass : public RenderPass {
   public:
    ~ModelRenderPass();

    void Init();

    void Draw() override;

    void SetScene(ScenePtr scene);
    void SetCamera(CameraPtr camera);
   private:
    void InitGeometry();
    void InitShader();

    ScenePtr m_Scene;
    std::vector<ModelPtr> m_Models;

    CameraPtr m_Camera;

    ProgramPtr m_Program;
    
    ShaderPtr m_VertexShader;
    ShaderSourcePtr m_VertexShaderSource;

    ShaderPtr m_FragmentShader;
    ShaderSourcePtr m_FragmentShaderSource;

    StatePtr m_RenderState;
    StatePtr m_PrevState;
};
using ModelRenderPassPtr = std::shared_ptr<ModelRenderPass>;

}  // namespace sid