#include "simple_rp.h"

#include "spdlog/spdlog.h"

namespace sid {

SimpleRenderPass::~SimpleRenderPass(){ 
    // Correct utilization order
    m_Program = nullptr;

    m_VertexShader.reset(nullptr);
    m_VertexShaderSource.reset(nullptr);

    m_FragmentShader.reset(nullptr);
    m_FragmentShaderSource.reset(nullptr);
}

void SimpleRenderPass::Draw() {
    static std::array<gl::GLfloat, 12> vertices = {
        0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
    static std::array<gl::GLuint, 6> indices = {0, 1, 3, 1, 2, 3};
    
    m_VBO->setData(vertices, gl::GL_STATIC_DRAW);
    m_EBO->setData(indices, gl::GL_STATIC_DRAW);

    m_Program->use();

    m_Position->setBuffer(m_VBO.get(), 0, 3 * sizeof(gl::GL_FLOAT));

    m_VAO->drawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);

    m_Program->release();

    m_VAO->unbind();
}

void SimpleRenderPass::Init() {
    InitGeometry();
    InitShader();
};

void SimpleRenderPass::InitGeometry() {
    m_VAO = make_vertex_array();

    m_VBO = make_buffer();
    m_EBO = make_buffer();
    
    m_VAO->bindElementBuffer(m_EBO.get());

    m_Position = m_VAO->binding(0);
    m_Position->setAttribute(0);
    m_Position->setFormat(3, gl::GL_FLOAT, gl::GL_TRUE, 0);
    
    m_VAO->enable(0);
}

void SimpleRenderPass::InitShader() {
    m_Program = sid::make_program();

    std::tie(m_VertexShader, m_VertexShaderSource) =
        sid::make_shader(gl::GL_VERTEX_SHADER, "simple_vertex.vs");
    std::tie(m_FragmentShader, m_FragmentShaderSource) =
        sid::make_shader(gl::GL_FRAGMENT_SHADER, "simple_fragment.fs");

    m_Program->attach(m_VertexShader.get());
    m_Program->attach(m_FragmentShader.get());

    m_Program->link();

    if(!m_Program->isLinked() || !m_Program->isValid()) {
        spdlog::info(m_Program->infoLog());
    }
}
}  // namespace sid
