#include "gui_render_pass.h"

// Some math
#include "glm/ext/matrix_clip_space.hpp"
#include "spdlog/spdlog.h"

namespace sid {

GUIRenderPass::~GUIRenderPass() {
    // Correct utilization order
    m_Program = nullptr;

    m_VertexShader.reset(nullptr);
    m_VertexShaderSource.reset(nullptr);

    m_FragmentShader.reset(nullptr);
    m_FragmentShaderSource.reset(nullptr);
}

void GUIRenderPass::Init(GLFWwindow* window) {
    ImGui::StyleColorsDark();

    InitShaders();
    InitFontTexture();
    InitBuffers();

    m_Window = window;
}

void GUIRenderPass::AddPanel(GUIPanelPtr panel) { m_Panels.push_back(panel); }

void GUIRenderPass::UpdateUIClock() {
    ImGuiIO& io = ImGui::GetIO();

    int w = 0, h = 0;
    int display_w = 0, display_h = 0;
    glfwGetWindowSize(m_Window, &w, &h);
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));

    double current_time = glfwGetTime();
    io.DeltaTime = m_TimeGlobal > 0.0 ? static_cast<float>(current_time - m_TimeGlobal)
                                      : 1.0f / 60.0f;
    m_TimeGlobal = current_time;

    if (w > 0 && h > 0) {
        io.DisplayFramebufferScale =
            ImVec2(static_cast<float>(display_w) / w, static_cast<float>(display_h) / h);
    }
}

void GUIRenderPass::Draw() {
    UpdateUIClock();

    SetRenderState();

    ImGui::NewFrame();

    for(auto& panel: m_Panels) {
        auto is_visible = panel->IsVisible();
        auto name = panel->GetName();
        auto flags = panel->GetFlags();
        ImGui::Begin(name, is_visible, flags);
        panel->Draw();
        ImGui::End();
    }

    ImGui::Render();

    auto draw_data = ImGui::GetDrawData();

    RenderDrawData(draw_data);

    RevertRenderState();
}

void GUIRenderPass::RenderDrawData(ImDrawData* data) {
    int fb_width = (int)(data->DisplaySize.x * data->FramebufferScale.x);
    int fb_height = (int)(data->DisplaySize.y * data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0) return;

    gl::glViewport(0, 0, (gl::GLsizei)fb_width, (gl::GLsizei)fb_height);
    float L = data->DisplayPos.x;
    float R = data->DisplayPos.x + data->DisplaySize.x;
    float T = data->DisplayPos.y;
    float B = data->DisplayPos.y + data->DisplaySize.y;

    auto ortho_mat = glm::ortho(L, R, B, T);

    ImVec2 clip_off = data->DisplayPos;
    ImVec2 clip_scale = data->FramebufferScale;

    gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_FILL);

    for (int n = 0; n < data->CmdListsCount; n++) {
        const auto cmd_list = data->CmdLists[n];

        m_VBO->setData((gl::GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert),
                       (const gl::GLvoid*)cmd_list->VtxBuffer.Data, gl::GL_STREAM_DRAW);
        m_EBO->setData((gl::GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawIdx),
                       (const gl::GLvoid*)cmd_list->IdxBuffer.Data, gl::GL_STREAM_DRAW);

        m_Program->use();
        m_Program->setUniform("ProjMtx", ortho_mat);
        m_Program->setUniform("Texture", 0);

        m_VertexAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));
        m_UVAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));
        m_ColorAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            ImVec4 clip_rect;
            clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
            clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
            clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
            clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

            if (clip_rect.x < fb_width && clip_rect.y < fb_height &&
                clip_rect.z >= 0.0f && clip_rect.w >= 0.0f) {
                gl::glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w),
                              (int)(clip_rect.z - clip_rect.x),
                              (int)(clip_rect.w - clip_rect.y));

                m_Texture->bind();

                gl::GLenum mode = gl::GLenum::GL_TRIANGLES;
                gl::GLenum vert_type =
                    sizeof(ImDrawIdx) == 2 ? gl::GL_UNSIGNED_SHORT : gl::GL_UNSIGNED_INT;

                auto offset = (pcmd->IdxOffset * sizeof(ImDrawIdx));

                m_VAO->drawElements(mode, pcmd->ElemCount, vert_type,
                                    (void*)(intptr_t)offset);

                m_Texture->unbind();
            }
        }
        m_Program->release();
    }
    m_VAO->unbind();
}

void GUIRenderPass::InitFontTexture() {
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels = nullptr;
    int width = 0, height = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    m_Texture = sid::make_texture();
    m_Texture->bind();
    m_Texture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
    m_Texture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
    m_Texture->image2D(0, gl::GL_RGBA, width, height, 0, gl::GL_RGBA,
                       gl::GL_UNSIGNED_BYTE, pixels);

    io.Fonts->TexID = reinterpret_cast<ImTextureID>(m_Texture->textureHandle().handle());

    m_Texture->unbind();
}

void GUIRenderPass::InitShaders() {
    m_Program = sid::make_program();

    std::tie(m_VertexShader, m_VertexShaderSource) =
        sid::make_shader(gl::GL_VERTEX_SHADER, "imgui.vs");
    std::tie(m_FragmentShader, m_FragmentShaderSource) =
        sid::make_shader(gl::GL_FRAGMENT_SHADER, "imgui.fs");

    m_Program->attach(m_VertexShader.get());
    m_Program->attach(m_FragmentShader.get());

    m_Program->bindAttributeLocation(0, "Position");
    m_Program->bindAttributeLocation(1, "UV");
    m_Program->bindAttributeLocation(2, "Color");

    m_Program->link();

    if (!m_Program->isLinked() || !m_Program->isValid()) {
        spdlog::info("Program link: {0}", m_Program->infoLog());
    }

    m_GUIState = make_state(globjects::State::DeferredMode);

    m_GUIState->enable(gl::GL_BLEND);
    m_GUIState->enable(gl::GL_SCISSOR_TEST);

    // gl::glBlendEquation(gl::GL_FUNC_ADD); ??
    m_GUIState->blendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    m_GUIState->disable(gl::GL_CULL_FACE);
    m_GUIState->disable(gl::GL_DEPTH_TEST);
}

void GUIRenderPass::InitBuffers() {
    m_VAO = sid::make_vertex_array();

    m_EBO = sid::make_buffer();
    m_VBO = sid::make_buffer();

    m_VAO->bindElementBuffer(m_EBO.get());

    auto vbo_offset = IM_OFFSETOF(ImDrawVert, pos);
    auto uv_offset = IM_OFFSETOF(ImDrawVert, uv);
    auto color_offset = IM_OFFSETOF(ImDrawVert, col);

    const gl::GLuint vbo_idx = 0;
    const gl::GLuint uv_idx = 1;
    const gl::GLuint color_idx = 2;

    auto vbo_location = m_Program->getAttributeLocation("Position");
    m_VertexAttrib = m_VAO->binding(vbo_idx);
    m_VertexAttrib->setAttribute(vbo_location);
    m_VertexAttrib->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, vbo_offset);
    m_VAO->enable(vbo_idx);

    auto uv_location = m_Program->getAttributeLocation("UV");
    m_UVAttrib = m_VAO->binding(uv_idx);
    m_UVAttrib->setAttribute(uv_location);
    m_UVAttrib->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, uv_offset);
    m_VAO->enable(uv_idx);

    auto color_location = m_Program->getAttributeLocation("Color");
    m_ColorAttrib = m_VAO->binding(color_idx);
    m_ColorAttrib->setAttribute(color_location);
    m_ColorAttrib->setFormat(4, gl::GL_UNSIGNED_BYTE, gl::GL_TRUE, color_offset);
    m_VAO->enable(color_idx);

    m_VAO->unbind();
}

void GUIRenderPass::SetRenderState() {
    if (!m_PrevState) {
        m_PrevState = globjects::State::currentState();
    }

    m_GUIState->apply();
}

void GUIRenderPass::RevertRenderState() { m_PrevState->apply(); }
}  // namespace sid
