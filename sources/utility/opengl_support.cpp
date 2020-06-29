#include "opengl_support.h"

#include "glad/glad.h"

namespace sid {
bool OpenGLSupport::Init() { return static_cast<bool>(gladLoadGL()); }
}  // namespace sid
