#include "gui_render_pass.h"

// Some math
#include "glm/ext/matrix_clip_space.hpp"
#include "spdlog/spdlog.h"

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#undef GLFW_EXPOSE_NATIVE_WIN32
#endif

namespace sid {

static GLFWmousebuttonfun g_PrevUserCallbackMousebutton = NULL;
static GLFWscrollfun g_PrevUserCallbackScroll = NULL;
static GLFWkeyfun g_PrevUserCallbackKey = NULL;
static GLFWcharfun g_PrevUserCallbackChar = NULL;

void OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (g_PrevUserCallbackMousebutton != NULL) {
        g_PrevUserCallbackMousebutton(window, button, action, mods);
    }

    // if (action == GLFW_PRESS && button >= 0 && button <
    // IM_ARRAYSIZE(g_MouseJustPressed))
    //     g_MouseJustPressed[button] = true;
}

void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_PrevUserCallbackScroll != NULL) {
        g_PrevUserCallbackScroll(window, xoffset, yoffset);
    }

    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += static_cast<float>(xoffset);
    io.MouseWheel += static_cast<float>(yoffset);
}

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (g_PrevUserCallbackKey != NULL) {
        g_PrevUserCallbackKey(window, key, scancode, action, mods);
    }

    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS) {
        io.KeysDown[key] = true;
    } else if (action == GLFW_RELEASE) {
        io.KeysDown[key] = false;
    }

    // Modifiers are not reliable across systems
    io.KeyCtrl =
        io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
#ifdef _WIN32
    io.KeySuper = false;
#else
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
}

void OnChar(GLFWwindow* window, unsigned int c) {
    if (g_PrevUserCallbackChar != NULL) {
        g_PrevUserCallbackChar(window, c);
    }

    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(c);
}

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

    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

#ifdef _WIN32
    io.ImeWindowHandle = (void*)glfwGetWin32Window(window);
#endif

    GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);
    m_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_MouseCursors[ImGuiMouseCursor_TextInput] =
        glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    m_MouseCursors[ImGuiMouseCursor_ResizeNS] =
        glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    m_MouseCursors[ImGuiMouseCursor_ResizeEW] =
        glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    m_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    m_MouseCursors[ImGuiMouseCursor_ResizeAll] =
        glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_MouseCursors[ImGuiMouseCursor_ResizeNESW] =
        glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_MouseCursors[ImGuiMouseCursor_ResizeNWSE] =
        glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_MouseCursors[ImGuiMouseCursor_NotAllowed] =
        glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

    g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, OnMouseButton);
    g_PrevUserCallbackScroll = glfwSetScrollCallback(window, OnMouseScroll);
    g_PrevUserCallbackKey = glfwSetKeyCallback(window, OnKey);
    g_PrevUserCallbackChar = glfwSetCharCallback(window, OnChar);

    InitShaders();
    InitFontTexture();
    InitBuffers();

    m_Window = window;
}

void GUIRenderPass::SetLayout(GUILayoutPtr layout) {
    m_Layout = layout;
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

    if (w > 0 && h > 0) {
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);
    }

    UpdateMouseState();
    UpdateMouseCursor();

    SetRenderState();

    ImGui::NewFrame();

    if(m_Layout!= nullptr) {
        m_Layout->Draw();
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
    m_VertexAttrib.reset(m_VAO->binding(vbo_idx));
    m_VertexAttrib->setAttribute(vbo_location);
    m_VertexAttrib->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, vbo_offset);
    m_VAO->enable(vbo_idx);

    auto uv_location = m_Program->getAttributeLocation("UV");
    m_UVAttrib.reset(m_VAO->binding(uv_idx));
    m_UVAttrib->setAttribute(uv_location);
    m_UVAttrib->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, uv_offset);
    m_VAO->enable(uv_idx);

    auto color_location = m_Program->getAttributeLocation("Color");
    m_ColorAttrib.reset(m_VAO->binding(color_idx));
    m_ColorAttrib->setAttribute(color_location);
    m_ColorAttrib->setFormat(4, gl::GL_UNSIGNED_BYTE, gl::GL_TRUE, color_offset);
    m_VAO->enable(color_idx);

    m_VAO->unbind();
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

void GUIRenderPass::UpdateMouseState() {
    // Update buttons
    ImGuiIO& io = ImGui::GetIO();
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
        // If a mouse press event came, always pass it as "mouse held this frame", so we
        // don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = m_MousePressed[i] || glfwGetMouseButton(m_Window, i) != 0;
        m_MousePressed[i] = false;
    }

    // Update mouse position
    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
#ifdef __EMSCRIPTEN__
    const bool focused = true;  // Emscripten
#else
    const bool focused = glfwGetWindowAttrib(m_Window, GLFW_FOCUSED) != 0;
#endif
    if (focused) {
        if (io.WantSetMousePos) {
            glfwSetCursorPos(m_Window, (double)mouse_pos_backup.x,
                             (double)mouse_pos_backup.y);
        } else {
            double mouse_x, mouse_y;
            glfwGetCursorPos(m_Window, &mouse_x, &mouse_y);
            io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
        }
    }
}
void GUIRenderPass::UpdateMouseCursor() {
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) ||
        glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else {
        // Show OS mouse cursor
        // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with
        // GLFW 3.2, but 3.3 works here.
        glfwSetCursor(m_Window, m_MouseCursors[imgui_cursor]
                                    ? m_MouseCursors[imgui_cursor]
                                    : m_MouseCursors[ImGuiMouseCursor_Arrow]);
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
}  // namespace sid
