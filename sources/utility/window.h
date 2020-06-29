#pragma once

#include "GLFW/glfw3.h"

namespace sid {
class CWindow {
 public:
  void Run();

 private:
  GLFWwindow* m_Window;
};
}  // namespace sid
