#pragma once

#include "utility/window.h"

// UI library
#include "imgui.h"

// Event/IO library
#include "GLFW/glfw3.h" 

namespace sid {
class Controls {
   public:
    void Init(Window* window);

    void UpdateMouseState();
    void UpdateMouseCursor();
   private:
    std::array<bool, ImGuiMouseButton_COUNT> m_MousePressed;
    std::array<GLFWcursor*, ImGuiMouseCursor_COUNT> m_MouseCursors;

    GLFWwindow* m_Window;
};
}  // namespace sid