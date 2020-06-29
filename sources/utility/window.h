#pragma once

#include "opengl_support.h"

// Must be included after all other opengl includes
#include "GLFW/glfw3.h"

namespace sid {
class Window {
 public:
  void Run();

 private:
  GLFWwindow* m_Window;
};
}  // namespace sid
