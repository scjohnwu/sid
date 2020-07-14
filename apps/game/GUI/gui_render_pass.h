#include "render/render_pass.h"
#include "utility/opengl_support.h"
#include "glsl/program_builder.h"

namespace game {
class GUIRenderPass : public sid::RenderPass {
   public:
    void Init();
    void Draw() override;

   private:
    bool InitFontTexture();
    bool InitShaders();

    GLuint m_FontTexture;
    sid::ProgramPtr m_GUIProgram;
};
}  // namespace game