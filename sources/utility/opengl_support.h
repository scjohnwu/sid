#pragma once

#include "glad/glad.h"
// Must be last one to include
#include "GLFW/glfw3.h"

namespace sid {
class OpenGLSupport {
   public:
    static bool LoadCore();
    static void EnableVSync();
    static bool LoadExtensions();
    static void TerminateCore();
};
}  // namespace sid