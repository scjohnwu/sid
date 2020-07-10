#include "glsl/program_builder.h"
#include "utility/window.h"
#include "render/render.h"

#include "GUI/gui_render_pass.h"

int main(int arch, const char** argv) {
    sid::OpenGLSupport::LoadCore();

    sid::Window window(640, 480, "Shooting in the dark");

    if (!window.IsValid()) {
        sid::OpenGLSupport::TerminateCore();
        return -1;
    }

    if (!sid::OpenGLSupport::LoadExtensions()) {
        sid::OpenGLSupport::TerminateCore();
        return -1;
    }

    sid::OpenGLSupport::EnableVSync();

    sid::ProgramBuilder builder;

    auto simple_program = builder.NewProgram()
                              .AddShader("simple_vertex.vs", sid::ShaderType::Vertex)
                              .AddShader("simple_fragment.fs", sid::ShaderType::Fragment)
                              .Build();

    // TODO:: move into class
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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