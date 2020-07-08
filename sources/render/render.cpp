#include "render.h"
#include "glad/glad.h"

namespace sid {
    void Render::Draw() {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glPointSize(30.0f);
    }
}