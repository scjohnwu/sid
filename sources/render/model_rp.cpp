#include "model_rp.h"

#include "spdlog/spdlog.h"
#include "glm/gtc/matrix_transform.hpp"

namespace sid {
ModelRenderPass::~ModelRenderPass() {
    // Correct utilization order
    m_Program = nullptr;

    m_VertexShader.reset(nullptr);
    m_VertexShaderSource.reset(nullptr);

    m_FragmentShader.reset(nullptr);
    m_FragmentShaderSource.reset(nullptr);
}

void ModelRenderPass::SetModel(ModelPtr model) { m_RenderModel = model; }

void ModelRenderPass::Draw() {
    if(!m_PrevState) {
        m_PrevState = globjects::State::currentState();
    }

   auto proj = glm::perspective( 45.0f, (float)1280/(float)720, 0.01f, 1000.0f);

    auto trans_mat = glm::identity<glm::mat4>();
    trans_mat = glm::scale(trans_mat, glm::vec3(0.01f, 0.01f, 0.01f));
    trans_mat = glm::translate(trans_mat, glm::vec3(0, 0, -12.0f));
    trans_mat = glm::rotate(trans_mat, 90.0f, glm::vec3(1.0f, 1.0f, 0.0f));

    m_RenderState->apply();

    m_Program->use();
    m_Program->setUniform("transform", trans_mat);
    m_Program->setUniform("projection", proj);

    m_RenderModel->Draw();

    m_Program->release();

    m_PrevState->apply();
}

void ModelRenderPass::Init() {
    InitGeometry();
    InitShader();

    m_RenderState = make_state(globjects::State::Mode::DeferredMode);
    m_RenderState->polygonMode(gl::GL_FRONT_AND_BACK, gl::GL_LINE);
};

void ModelRenderPass::InitGeometry() {
    m_VAO = make_vertex_array();

    m_VBO = make_buffer();
    m_EBO = make_buffer();

    m_VAO->bindElementBuffer(m_EBO.get());

    m_Position = m_VAO->binding(0);
    m_Position->setAttribute(0);
    m_Position->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);

    m_VAO->enable(0);
}

void ModelRenderPass::InitShader() {
    m_Program = sid::make_program();

    std::tie(m_VertexShader, m_VertexShaderSource) =
        sid::make_shader(gl::GL_VERTEX_SHADER, "model.vs");
    std::tie(m_FragmentShader, m_FragmentShaderSource) =
        sid::make_shader(gl::GL_FRAGMENT_SHADER, "model.fs");

    m_Program->attach(m_VertexShader.get());
    m_Program->attach(m_FragmentShader.get());

    m_Program->link();

    if (!m_Program->isLinked() || !m_Program->isValid()) {
        spdlog::info(m_Program->infoLog());
    }
}
}  // namespace sid
