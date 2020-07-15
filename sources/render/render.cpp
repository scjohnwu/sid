#include "render.h"
#include "utility/opengl_support.h"
#include "spdlog/spdlog.h"

namespace sid {
    void Render::Draw() {
        gl::glClearColor(0.0, 0.0, 0.0, 1.0);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);
        gl::glClear(gl::GL_DEPTH_BUFFER_BIT);

        for( auto& pass: m_Passes ) {
            pass->Draw();
        }
    }

    void Render::AddPass(RenderPassPtr render_pass) {
        m_Passes.push_back(render_pass);
        
        spdlog::info("New render pass added");
    }
}