#include "model_rp.h"

#include "glm/gtc/matrix_transform.hpp"
#include "spdlog/spdlog.h"


namespace sid {
ModelRenderPass::~ModelRenderPass() {
    // Correct utilization order
    m_Program = nullptr;

    m_VertexShader.reset(nullptr);
    m_VertexShaderSource.reset(nullptr);

    m_FragmentShader.reset(nullptr);
    m_FragmentShaderSource.reset(nullptr);
}

void ModelRenderPass::Draw() {
    if (!m_PrevState) {
        m_PrevState = globjects::State::currentState();
    }

    auto proj_mat = m_Camera->GetProjMat();
    auto view_mat = m_Camera->GetViewMat();

    m_RenderState->apply();
    m_Program->use();

    // ~constant
    m_Program->setUniform("projection", proj_mat);
    // from camera
    m_Program->setUniform("view", view_mat);

    for (auto& model : m_Models) {
        auto model_mat = model->GetWorldMat();

        // from entity
        m_Program->setUniform("model", model_mat);
        model->Draw();
    }

    m_Program->release();
    m_PrevState->apply();
}

void ModelRenderPass::Init() {
    InitShader();

    m_RenderState = make_state(globjects::State::Mode::DeferredMode);
    m_RenderState->polygonMode(gl::GL_FRONT_AND_BACK, gl::GL_LINE);
};

void ModelRenderPass::SetScene(ScenePtr scene) {
    m_Scene = scene;
    m_Models = m_Scene->GetRenderModels();
}

void ModelRenderPass::SetCamera(CameraPtr camera) { m_Camera = camera; }

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
