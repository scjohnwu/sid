#include "gui_render_pass.h"

// Some math
#include "glm/ext/matrix_clip_space.hpp"
#include "spdlog/spdlog.h"

namespace game {

void GUIRenderPass::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    InitShaders();
    InitFontTexture();
    InitBuffers();

    m_Window = window;
}

void GUIRenderPass::Draw() {
    ImGuiIO& io = ImGui::GetIO();

    int w = 0, h = 0;
    int display_w = 0, display_h = 0;
    glfwGetWindowSize(m_Window, &w, &h);
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // SetRenderState();

    ImGui::NewFrame();

    if (m_ShowDemoWindow) {
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);
    }

    ImGui::Render();

    auto draw_data = ImGui::GetDrawData();

    RenderDrawData(draw_data);

    // RevertRenderState();
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

    // TODO:: set uniforms

    // auto tex_location = m_Program->getUniformLocation("Texture");
    // m_Program->setUniform(tex_location, 0);
    ImVec2 clip_off = data->DisplayPos;
    ImVec2 clip_scale = data->FramebufferScale;

    gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_FILL);

    for (int n = 0; n < data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = data->CmdLists[n];

        m_VBO->setData((gl::GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert),
                       (const gl::GLvoid*)cmd_list->VtxBuffer.Data, gl::GL_STREAM_DRAW);
        m_EBO->setData((gl::GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawIdx),
                       (const gl::GLvoid*)cmd_list->IdxBuffer.Data, gl::GL_STREAM_DRAW);

        m_Program->use();
        m_Program->setUniform("ProjMtx", ortho_mat);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            gl::GLenum mode = gl::GLenum::GL_TRIANGLES;
            gl::GLenum vert_type =
                sizeof(ImDrawIdx) == 2 ? gl::GL_UNSIGNED_SHORT : gl::GL_UNSIGNED_INT;

            auto offset = (pcmd->IdxOffset * sizeof(ImDrawIdx));

            m_VAO->drawElements(mode, pcmd->ElemCount, vert_type,
                                (void*)(intptr_t)offset);
        }

        m_Program->release();
    }
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

    gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGBA, width, height, 0, gl::GL_RGBA,
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

    m_Program->link();

    spdlog::info("Program link: {0}", m_Program->infoLog());
}

void GUIRenderPass::InitBuffers() {
    m_VAO = sid::make_vertex_array();

    m_EBO = sid::make_buffer();
    m_VAO->bindElementBuffer(m_EBO.get());

    m_VBO = sid::make_buffer();
    auto vbo_location = 0;
    m_VertexAttrib.reset(m_VAO->binding(vbo_location));
    m_VertexAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));
    m_VertexAttrib->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE,
                              IM_OFFSETOF(ImDrawVert, pos));
    m_VAO->enable(vbo_location);

    // auto uv_location = m_Program->getAttributeLocation("UV");
    // m_UVAttrib.reset(m_VAO->binding(uv_location));
    // m_UVAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));
    // m_UVAttrib->setFormat(2, gl::GL_FLOAT, gl::GL_TRUE, IM_OFFSETOF(ImDrawVert, uv));
    // m_VAO->enable(uv_location);

    // auto color_location = 1;
    // m_ColorAttrib.reset(m_VAO->binding(color_location));
    // m_ColorAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));
    // m_ColorAttrib->setFormat(4, gl::GL_UNSIGNED_BYTE, gl::GL_TRUE,
    //                          IM_OFFSETOF(ImDrawVert, col));
    // m_VAO->enable(color_location);
}

void GUIRenderPass::SetRenderState() {
    gl::glEnable(gl::GL_BLEND);
    gl::glBlendEquation(gl::GL_FUNC_ADD);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    gl::glDisable(gl::GL_CULL_FACE);
    gl::glDisable(gl::GL_DEPTH_TEST);
    gl::glEnable(gl::GL_SCISSOR_TEST);
}

void GUIRenderPass::RevertRenderState() {
    gl::glEnable(gl::GL_CULL_FACE);
    gl::glEnable(gl::GL_DEPTH_TEST);
    gl::glDisable(gl::GL_SCISSOR_TEST);
}
}  // namespace game
