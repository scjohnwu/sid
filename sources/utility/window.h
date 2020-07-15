#pragma once

#include <string>

#include "opengl_support.h"

namespace sid {
class Window {
   public:
    Window(unsigned int width, unsigned int height, std::string caption);

    void SwapBuffers();
    void PollEvents();

    bool IsValid() const;
    bool IsNotClosing() const;

    GLFWwindow* get() const;
   private:
    GLFWwindow* m_Window;
    bool m_IsValid{false};
};
}  // namespace sid
