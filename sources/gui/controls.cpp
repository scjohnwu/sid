#include "controls.h"

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

void Controls::Init(Window* window) {
    ImGuiIO& io = ImGui::GetIO();

    m_Window = window->get();

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
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
    io.ImeWindowHandle = (void*)glfwGetWin32Window(m_Window);
#endif

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

    g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(m_Window, OnMouseButton);
    g_PrevUserCallbackScroll = glfwSetScrollCallback(m_Window, OnMouseScroll);
    g_PrevUserCallbackKey = glfwSetKeyCallback(m_Window, OnKey);
    g_PrevUserCallbackChar = glfwSetCharCallback(m_Window, OnChar);
}

void Controls::UpdateMouseState() {
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
void Controls::UpdateMouseCursor() {
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