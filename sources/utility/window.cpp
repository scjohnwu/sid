#include "window.h"

namespace sid {
Window::Window(unsigned int width, unsigned int height, std::string caption) {
    m_Window = glfwCreateWindow(width, height, caption.c_str(), nullptr, nullptr);

    if (m_Window) {
        glfwMakeContextCurrent(m_Window);
        m_IsValid = true;
    }
}

void Window::SwapBuffers() { glfwSwapBuffers(m_Window); }

void Window::PollEvents() { glfwPollEvents(); }

bool Window::IsValid() const { return m_IsValid; }

GLFWwindow* Window::get() const { return m_Window; }

bool Window::IsNotClosing() const { return !glfwWindowShouldClose(m_Window); }

}  // namespace sid
