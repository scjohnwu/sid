#pragma once
#include "render/render_pass.h"
// All render stuff
#include "utility/opengl_support.h"
// UI library
#include "imgui.h"

#include "gui_panel.h"

#include <array>

namespace sid {
class GUIRenderPass : public sid::RenderPass {
   public:
    ~GUIRenderPass();

    void Init(GLFWwindow* window);
    void Draw() override;

    void AddPanel(GUIPanelPtr panel);

   private:
    void InitFontTexture();
    void InitShaders();
    void InitBuffers();

    void SetRenderState();
    void RevertRenderState();

    void UpdateUIClock();

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

    sid::StatePtr m_PrevState;
    sid::StatePtr m_GUIState;

    std::vector<GUIPanelPtr> m_Panels;

    sid::TexturePtr m_Texture;
    double m_TimeGlobal{0.0};

    GLFWwindow* m_Window{nullptr};
};
}  // namespace game