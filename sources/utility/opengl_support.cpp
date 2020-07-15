#include "opengl_support.h"

#include "globjects/base/StaticStringSource.h"
#include "globjects/base/File.h"
#include "globjects/globjects.h"

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
    globjects::setCurrentContext();

    return true;
}

void OpenGLSupport::TerminateCore() { glfwTerminate(); }

ProgramPtr make_program() { return std::make_shared<globjects::Program>(); }
ShaderPtr make_shader(const gl::GLenum type, globjects::AbstractStringSource* source) {
    return globjects::Shader::create(type, source);
}

ShaderPtr make_shader(const gl::GLenum type, std::string filename) { 
    auto file_source = globjects::Shader::sourceFromFile(filename);
    auto string_source = globjects::Shader::sourceFromString(file_source->string());

    auto result = make_shader(type, string_source.get());
    result->removeSubject(string_source.get());
    result->setSource(nullptr);
    
    return std::move(result);
}

BufferPtr make_buffer() {
    return std::make_shared<globjects::Buffer>();
}

VertexArrayPtr make_vertex_array() {
    return std::make_shared<globjects::VertexArray>();
}

TexturePtr make_texture() {
    return std::make_shared<globjects::Texture>();
}

}  // namespace sid