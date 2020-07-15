#include "gui_render_pass.h"
#include "imgui.h"
#include "utility/opengl_support.h"

namespace game {

void GUIRenderPass::Init() {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    InitShaders();
    InitFontTexture();
}
void GUIRenderPass::Draw() {
    // State config
    glEnable(GL_BLEND);
    gl::glBlendEquation(gl::GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);

    // View config
    // glViewport


    // Do stuff

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
}

bool GUIRenderPass::InitFontTexture() {
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels = nullptr;
    int width = 0, height = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    glGenTextures(1, &m_FontTexture);
    glBindTexture(GL_TEXTURE_2D, m_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    io.Fonts->TexID = reinterpret_cast<ImTextureID>(m_FontTexture);

    // TODO:: error checks
    return true;
}

bool GUIRenderPass::InitShaders() {
    m_Program = sid::make_program();
    
    auto vertex_shader = sid::make_shader(gl::GL_VERTEX_SHADER, "imgui.vs");
    auto fragment_shader = sid::make_shader(gl::GL_FRAGMENT_SHADER, "imgui.fs");
    
    m_Program->attach(vertex_shader.get());
    m_Program->attach(fragment_shader.get());
    
    m_Program->link();

    return true;
}
}  // namespace game
