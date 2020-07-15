#include "opengl_support.h"

#include "globjects/base/StaticStringSource.h"
#include "globjects/base/File.h"

namespace sid {
bool OpenGLSupport::LoadCore() {
    auto result = static_cast<bool>(glfwInit());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    return result;
}

void OpenGLSupport::EnableVSync() { glfwSwapInterval(1); }

bool OpenGLSupport::LoadExtensions() {
    glbinding::initialize(glfwGetProcAddress);
    return true;
}

void OpenGLSupport::TerminateCore() { glfwTerminate(); }

ProgramPtr make_program() { return std::make_shared<globjects::Program>(); }
ShaderPtr make_shader(const gl::GLenum type, globjects::AbstractStringSource* source) {
    return std::make_unique<globjects::Shader>(type, source);
}

ShaderPtr make_shader(const gl::GLenum type, std::string filename) { 
    auto file_source = globjects::Shader::sourceFromFile(filename);
    auto string_source = globjects::Shader::sourceFromString(file_source->string());

    return make_shader(type, string_source.get());
}

}  // namespace sid