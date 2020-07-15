#include "simple_rp.h"

#include "spdlog/spdlog.h"

namespace sid {
void SimpleRenderPass::Draw() {
    static std::array<gl::GLfloat, 12> vertices = {
        0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
    static std::array<gl::GLuint, 6> indices = {0, 1, 3, 1, 2, 3};

    m_VBO->setData(vertices, gl::GL_STATIC_DRAW);
    m_EBO->setData(indices, gl::GL_STATIC_DRAW);

    m_Program->use();

    m_VAO->drawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);

    m_Program->release();
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

    auto vert_binding = m_VAO->binding(0);
    vert_binding->setAttribute(0);
    vert_binding->setBuffer(m_VBO.get(), 0, 3 * sizeof(gl::GL_FLOAT));
    vert_binding->setFormat(3, gl::GL_FLOAT, gl::GL_TRUE, 0);
    m_VAO->enable(0);

    spdlog::info("Vao isDefault: {0}", m_VAO->isDefault());
    spdlog::info("Vao: {0}", m_VAO->name());
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

    auto link_info = m_Program->infoLog();

    spdlog::info(link_info);

    auto valid = m_Program->isValid();

    spdlog::info(valid);
}
}  // namespace sid
