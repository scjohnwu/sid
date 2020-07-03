#include "opengl_support.h"

namespace sid {
bool OpenGLSupport::LoadCore() {
    auto result = static_cast<bool>(glfwInit());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    return result;
}

void OpenGLSupport::EnableVSync() { glfwSwapInterval(1); }

bool OpenGLSupport::LoadExtensions() { return static_cast<bool>(gladLoadGL()); }

void OpenGLSupport::TerminateCore() { glfwTerminate(); }
}  // namespace sid