#include "gui_render_pass.h"

// Some math
#include "glm/ext/matrix_clip_space.hpp"

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

    // State config
    gl::glEnable(gl::GL_BLEND);
    gl::glBlendEquation(gl::GL_FUNC_ADD);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
    gl::glDisable(gl::GL_CULL_FACE);
    gl::glDisable(gl::GL_DEPTH_TEST);
    gl::glEnable(gl::GL_SCISSOR_TEST);

    ImGui::NewFrame();

    if (m_ShowDemoWindow) {
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);
    }

    ImGui::Render();

    auto draw_data = ImGui::GetDrawData();

    RenderDrawData(draw_data);

    gl::glEnable(gl::GL_CULL_FACE);
    gl::glEnable(gl::GL_DEPTH_TEST);
    gl::glDisable(gl::GL_SCISSOR_TEST);
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

    m_Program->use();
    // TODO:: set uniforms
    m_Program->setUniform("ProjMtx", ortho_mat);

    auto tex_location = m_Program->getUniformLocation("Texture");
    m_Program->setUniform(tex_location, 0);

    m_VAO->bind();
    m_VBO->bind(gl::GL_ARRAY_BUFFER);
    m_EBO->bind(gl::GL_ELEMENT_ARRAY_BUFFER);

    ImVec2 clip_off = data->DisplayPos;
    ImVec2 clip_scale = data->FramebufferScale;

    for (int n = 0; n < data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = data->CmdLists[n];

        m_VBO->setData((gl::GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert),
                       (const gl::GLvoid*)cmd_list->VtxBuffer.Data, gl::GL_STREAM_DRAW);
        m_EBO->setData((gl::GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawIdx),
                       (const gl::GLvoid*)cmd_list->IdxBuffer.Data, gl::GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != nullptr) {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by
                // the user to request the renderer to reset render state.)
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height &&
                    clip_rect.z >= 0.0f && clip_rect.w >= 0.0f) {
                    // Apply scissor/clipping rectangle
                    gl::glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w),
                                  (int)(clip_rect.z - clip_rect.x),
                                  (int)(clip_rect.w - clip_rect.y));

                    
                    m_Texture->bind();

                    gl::GLenum mode = gl::GLenum::GL_TRIANGLES;
                    gl::GLenum vert_type = sizeof(ImDrawIdx) == 2 ? gl::GL_UNSIGNED_SHORT
                                                                  : gl::GL_UNSIGNED_INT;

                    m_VAO->drawElements(mode, pcmd->ElemCount, vert_type, nullptr);

                    m_Texture->unbind();
                }
            }
        }
    }

    m_VBO->unbind(gl::GL_ARRAY_BUFFER);
    m_EBO->unbind(gl::GL_ELEMENT_ARRAY_BUFFER);
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

    auto vertex_shader = sid::make_shader(gl::GL_VERTEX_SHADER, "imgui.vs");
    auto fragment_shader = sid::make_shader(gl::GL_FRAGMENT_SHADER, "imgui.fs");

    m_Program->attach(vertex_shader.get());
    m_Program->attach(fragment_shader.get());

    m_Program->link();
}

void GUIRenderPass::InitBuffers() {
    m_VAO = sid::make_vertex_array();

    m_VBO = sid::make_buffer();
    m_EBO = sid::make_buffer();

    auto vbo_location = m_Program->getAttributeLocation("Position");
    m_VAO->enable(vbo_location);
    m_PosAttrib.reset(m_VAO->binding(vbo_location));
    m_PosAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));
    m_PosAttrib->setFormat(sizeof(ImDrawVert), gl::GL_FLOAT, false,
                           IM_OFFSETOF(ImDrawVert, pos));

    auto uv_location = m_Program->getAttributeLocation("UV");
    m_VAO->enable(uv_location);
    m_UVAttrib.reset(m_VAO->binding(uv_location));
    m_UVAttrib->setFormat(sizeof(ImDrawVert), gl::GL_FLOAT, false,
                          IM_OFFSETOF(ImDrawVert, uv));

    auto color_location = m_Program->getAttributeLocation("Color");
    m_VAO->enable(color_location);
    m_ColorAttrib.reset(m_VAO->binding(color_location));
    m_ColorAttrib->setFormat(sizeof(ImDrawVert), gl::GL_UNSIGNED_BYTE, true,
                             IM_OFFSETOF(ImDrawVert, col));
}
}  // namespace game
