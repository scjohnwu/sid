#include "utility/window.h"
#include "render/render.h"

#include "GUI/gui_render_pass.h"
#include "spdlog/spdlog.h"

int main(int arch, const char** argv) {
    sid::OpenGLSupport::LoadCore();

    sid::Window window(640, 480, "Shooting in the dark");

    if (!window.IsValid()) {
        spdlog::critical("Could not create window");
        sid::OpenGLSupport::TerminateCore();
        return -1;
    }

    if (!sid::OpenGLSupport::LoadExtensions()) {
        spdlog::critical("Could not load opengl extensions");
        sid::OpenGLSupport::TerminateCore();
        return -1;
    }

    spdlog::info("Application essentials have been initialized");

    sid::OpenGLSupport::EnableVSync();

    // TODO:: move into class
    gl::GLuint vao;
    gl::glGenVertexArrays(gl::GLsizei{1}, &vao);
    gl::glBindVertexArray(vao);

    sid::RenderPassPtr gui_pass = std::make_shared<game::GUIRenderPass>();
    sid::Render render;
    render.AddPass(gui_pass);

    while (window.IsNotClosing()) {
        render.Draw();

        window.SwapBuffers();
        window.PollEvents();
    }

    sid::OpenGLSupport::TerminateCore();

    return 0;
}