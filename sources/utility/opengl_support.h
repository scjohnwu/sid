#pragma once

// glbindings for core profile 4.3
#include "glbinding/gl43core/gl.h"
#include "glbinding/glbinding.h"

// globjects imports, used for aliases
#include "globjects/Program.h"
#include "globjects/Shader.h"
#include "globjects/VertexArray.h"
#include "globjects/Buffer.h"
#include "globjects/VertexAttributeBinding.h"

// Don't include gl.h manually
#define GLFW_INCLUDE_NONE
// window/event routines and function loader
#include "GLFW/glfw3.h"

// gl math library
#include "glm/glm.hpp"

namespace sid {
class OpenGLSupport {
   public:
    static bool LoadCore();
    static void EnableVSync();
    static bool LoadExtensions();
    static void TerminateCore();
};

// Pointer aliases. 
// globjects::Instantiator is not used, because:
// 1. Verbose
// 2. Different owning types for different pointers
using ProgramPtr = std::shared_ptr<globjects::Program>;
ProgramPtr make_program();

using ShaderPtr = std::unique_ptr<globjects::Shader>;
ShaderPtr make_shader(const gl::GLenum type, globjects::AbstractStringSource * source);
ShaderPtr make_shader(const gl::GLenum type, std::string filename);

using BufferPtr = std::shared_ptr<globjects::Buffer>;
BufferPtr make_buffer();

using VertexArrayPtr = std::shared_ptr<globjects::VertexArray>;
VertexArrayPtr make_vertex_array();

using VertexAttribPtr = std::unique_ptr<globjects::VertexAttributeBinding>;

}  // namespace sid