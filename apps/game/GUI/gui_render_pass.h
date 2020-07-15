#include "render/render_pass.h"
// All render stuff
#include "utility/opengl_support.h"
// UI library
#include "imgui.h"

namespace game {
class GUIRenderPass : public sid::RenderPass {
   public:
    void Init(GLFWwindow* window);
    void Draw() override;

   private:
    void InitFontTexture();
    void InitShaders();
    void InitBuffers();

    void RenderDrawData(ImDrawData* data);

    sid::ProgramPtr m_Program;

    sid::VertexArrayPtr m_VAO;
    sid::BufferPtr m_VBO;
    sid::BufferPtr m_EBO;

    sid::VertexAttribPtr m_PosAttrib;
    sid::VertexAttribPtr m_UVAttrib;
    sid::VertexAttribPtr m_ColorAttrib;

    sid::TexturePtr m_Texture;

    bool m_ShowDemoWindow { true };

    GLFWwindow* m_Window { nullptr };
};
}  // namespace game