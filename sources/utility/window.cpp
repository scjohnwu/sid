#include "window.h"

namespace sid {
void CWindow::Run() {
  if (!glfwInit()) return;

  m_Window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!m_Window) {
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(m_Window);

  while (!glfwWindowShouldClose(m_Window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(m_Window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
}
}  // namespace sid