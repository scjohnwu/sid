#include "gui_render_pass.h"

// Some math
#include "glm/ext/matrix_clip_space.hpp"
#include "spdlog/spdlog.h"

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#undef GLFW_EXPOSE_NATIVE_WIN32
#endif

namespace game {

void GUIRenderPass::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();

    io.BackendFlags |=
        ImGuiBackendFlags_HasMouseCursors;  // We can honor GetMouseCursor() values
                                            // (optional)
    io.BackendFlags |=
        ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests
                                           // (optional, rarely used)
    io.BackendPlatformName = "imgui_globjects_impl";

#ifdef _WIN32
    io.ImeWindowHandle = (void*)glfwGetWin32Window(window);
#endif

    InitShaders();
    InitFontTexture();
    InitBuffers();

    // globjects::Buffer::hintBindlessImplementation(globjects::Buffer::BindlessImplementation::Legacy);
    // globjects::Buffer::hintNameImplementation(globjects::Buffer::NameImplementation::Legacy);

    m_Window = window;
}

void GUIRenderPass::Draw() {
    ImGuiIO& io = ImGui::GetIO();

    int w = 0, h = 0;
    int display_w = 0, display_h = 0;
    glfwGetWindowSize(m_Window, &w, &h);
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)w, (float)h);

    double current_time = glfwGetTime();
    io.DeltaTime =
        m_TimeGlobal > 0.0 ? (float)(current_time - m_TimeGlobal) : (float)(1.0f / 60.0f);
    m_TimeGlobal = current_time;

    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    SetRenderState();

    ImGui::NewFrame();

    if (m_ShowDemoWindow) {
        //ImGui::ShowDemoWindow(&m_ShowDemoWindow);
                 static float f = 0.0f;
            static int counter = 0;
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &m_ShowDemoWindow);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &m_ShowDemoWindow);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

    // TODO:: set uniforms

    ImVec2 clip_off = data->DisplayPos;
    ImVec2 clip_scale = data->FramebufferScale;

    gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_FILL);


    // gl::glBindVertexArray(m_RawVAO);
    m_VAO->bind();

    for (int n = 0; n < data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = data->CmdLists[n];

        m_VBO->bind(gl::GL_VERTEX_ARRAY);
        m_VBO->setData((gl::GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert),
                       (const gl::GLvoid*)cmd_list->VtxBuffer.Data, gl::GL_STREAM_DRAW);
        m_EBO->bind(gl::GL_ELEMENT_ARRAY_BUFFER);
        m_EBO->setData((gl::GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawIdx),
                       (const gl::GLvoid*)cmd_list->IdxBuffer.Data, gl::GL_STREAM_DRAW);
   

        // "Common raw"
        // gl::glBindBuffer(gl::GL_ARRAY_BUFFER, m_RawVBO);
        // gl::glBufferData(
        //     gl::GL_ARRAY_BUFFER,
        //     (gl::GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert),
        //     (const gl::GLvoid*)cmd_list->VtxBuffer.Data, gl::GL_STREAM_DRAW);

        // gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_RawEBO);
        // gl::glBufferData(
        //     gl::GL_ELEMENT_ARRAY_BUFFER,
        //     (gl::GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawVert),
        //     (const gl::GLvoid*)cmd_list->IdxBuffer.Data, gl::GL_STREAM_DRAW);        

        m_Program->use();
        m_Program->setUniform("ProjMtx", ortho_mat);

        auto tex_location = m_Program->getUniformLocation("Texture");
        m_Program->setUniform(tex_location, 0);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            ImVec4 clip_rect;
            clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
            clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
            clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
            clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

            // "Modern"
            gl::glBindVertexBuffer(0, m_VBO->id(), 0, sizeof(ImDrawVert));
            gl::glBindVertexBuffer(1, m_VBO->id(), 0, sizeof(ImDrawVert));
            gl::glBindVertexBuffer(2, m_VBO->id(), 0, sizeof(ImDrawVert));

            // m_VertexAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));
            // m_UVAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));
            // m_ColorAttrib->setBuffer(m_VBO.get(), 0, sizeof(ImDrawVert));

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

                // m_VAO->drawElements(mode, pcmd->ElemCount, vert_type,
                //                     (void*)(intptr_t)offset);

                // Common Raw
                gl::glDrawElements(mode, (gl::GLsizei)pcmd->ElemCount, vert_type,
                                   (void*)(intptr_t)offset);
            }
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

    spdlog::info("Program link: {0}", m_Program->infoLog());
}

void GUIRenderPass::InitBuffers() {
    m_VAO = sid::make_vertex_array();

    m_EBO = sid::make_buffer();
    m_VBO = sid::make_buffer();

    // "Common raw"
    gl::glGenVertexArrays(1, &m_RawVAO);
    gl::glGenBuffers(1, &m_RawVBO);
    // gl::glGenBuffers(1, &m_RawEBO);
    // gl::glBindVertexArray(m_RawVAO);

    auto vbo_offset = IM_OFFSETOF(ImDrawVert, pos);
    auto uv_offset = IM_OFFSETOF(ImDrawVert, uv);
    auto color_offset = IM_OFFSETOF(ImDrawVert, col);

    auto vbo_location = m_Program->getAttributeLocation("Position");
    m_VertexAttrib.reset(m_VAO->binding(0));
    m_VertexAttrib->setAttribute(vbo_location);
    m_VertexAttrib->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, vbo_offset);
    m_VAO->enable(0);
    
    auto uv_location = m_Program->getAttributeLocation("UV");
    m_UVAttrib.reset(m_VAO->binding(1));
    m_UVAttrib->setAttribute(uv_location);
    m_UVAttrib->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, uv_offset);
    m_VAO->enable(1);

    auto color_location = m_Program->getAttributeLocation("Color");
    m_ColorAttrib.reset(m_VAO->binding(2));
    m_ColorAttrib->setAttribute(color_location);
    m_ColorAttrib->setFormat(4, gl::GL_UNSIGNED_BYTE, gl::GL_TRUE, color_offset);
    m_VAO->enable(2);

    m_VAO->unbind();

    // "Modern"
    // gl::glVertexAttribFormat(0, 2, gl::GL_FLOAT, false, vbo_offset );
    // gl::glVertexAttribBinding(0, 0);
    // gl::glEnableVertexAttribArray(0);

    // gl::glVertexAttribFormat(1, 2, gl::GL_FLOAT, false, uv_offset );
    // gl::glVertexAttribBinding(1, 1);
    // gl::glEnableVertexAttribArray(1);

    // gl::glVertexAttribFormat(2, 4, gl::GL_UNSIGNED_BYTE, true, color_offset);
    // gl::glVertexAttribBinding(2, 2);
    // gl::glEnableVertexAttribArray(2);

    // m_VAO->unbind();
    // gl::glBindVertexArray(0);

    // "Legacy"
    // gl::glBindBuffer(gl::GL_ARRAY_BUFFER, m_RawVBO);
    // gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_RawEBO);

    // gl::glEnableVertexAttribArray(0);
    // gl::glEnableVertexAttribArray(1);
    // gl::glEnableVertexAttribArray(2);

    // gl::glVertexAttribPointer(0, 2, gl::GL_FLOAT, gl::GL_FALSE, sizeof(ImDrawVert),
    //                           (gl::GLvoid*)vbo_offset);
    // gl::glVertexAttribPointer(1, 2, gl::GL_FLOAT, gl::GL_FALSE, sizeof(ImDrawVert),
    //                           (gl::GLvoid*)uv_offset);
    // gl::glVertexAttribPointer(2, 4, gl::GL_UNSIGNED_BYTE, gl::GL_TRUE, sizeof(ImDrawVert),
    //                           (gl::GLvoid*)color_offset);


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
