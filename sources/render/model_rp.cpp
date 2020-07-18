#include "model_rp.h"

#include "spdlog/spdlog.h"

namespace sid {
ModelRenderPass::~ModelRenderPass(){ 
    // Correct utilization order
    m_Program = nullptr;

    m_VertexShader.reset(nullptr);
    m_VertexShaderSource.reset(nullptr);

    m_FragmentShader.reset(nullptr);
    m_FragmentShaderSource.reset(nullptr);
}

void ModelRenderPass::SetModel(ModelPtr model) {
    m_RenderModel = model;
}

void ModelRenderPass::Draw() {
    m_Program->use();

    m_RenderModel->Draw();

    m_Program->release();
}

void ModelRenderPass::Init() {
    InitGeometry();
    InitShader();
};

void ModelRenderPass::InitGeometry() {
    m_VAO = make_vertex_array();

    m_VBO = make_buffer();
    m_EBO = make_buffer();
    
    m_VAO->bindElementBuffer(m_EBO.get());

    m_Position = m_VAO->binding(0);
    m_Position->setAttribute(0);
    m_Position->setFormat(3, gl::GL_FLOAT, gl::GL_TRUE, 0);
    
    m_VAO->enable(0);
}

void ModelRenderPass::InitShader() {
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
