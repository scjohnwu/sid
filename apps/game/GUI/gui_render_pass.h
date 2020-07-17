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

    void SetRenderState();
    void RevertRenderState();

    void RenderDrawData(ImDrawData* data);

    sid::ProgramPtr m_Program;

    sid::VertexArrayPtr m_VAO;
    sid::BufferPtr m_VBO;
    sid::BufferPtr m_EBO;

    sid::VertexAttribPtr m_VertexAttrib;
    sid::VertexAttribPtr m_UVAttrib;
    sid::VertexAttribPtr m_ColorAttrib;

    sid::ShaderPtr m_VertexShader;
    sid::ShaderSourcePtr m_VertexShaderSource;

    sid::ShaderPtr m_FragmentShader;
    sid::ShaderSourcePtr m_FragmentShaderSource;

    gl::GLuint m_RawVBO{0};
    gl::GLuint m_RawEBO{0};
    gl::GLuint m_RawVAO{0};

    sid::TexturePtr m_Texture;
    double m_TimeGlobal{0.0};

    bool m_ShowDemoWindow{true};

    GLFWwindow* m_Window{nullptr};
};
}  // namespace game