#include "glsl/program_builder.h"
#include "utility/window.h"

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

    while (window.IsNotClosing()) {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glPointSize(30.0f);
        simple_program->Apply();
        glDrawArrays(GL_POINTS, 0, 1);

        window.SwapBuffers();
        window.PollEvents();
    }

    sid::OpenGLSupport::TerminateCore();

    return 0;
}