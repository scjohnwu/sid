#include "render/render_pass.h"
#include "utility/opengl_support.h"
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

    gl::GLuint m_FontTexture;

    sid::ProgramPtr m_Program;

    sid::VertexArrayPtr m_VAO;
    sid::BufferPtr m_VBO;
    sid::BufferPtr m_EBO;

    sid::VertexAttribPtr m_PosAttrib;
    sid::VertexAttribPtr m_UVAttrib;
    sid::VertexAttribPtr m_ColorAttrib;

    bool m_ShowDemoWindow { true };
    GLFWwindow* m_Window;

};
}  // namespace game