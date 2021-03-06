#include "gui/controls.h"
#include "gui/gui_render_pass.h"
#include "gui/imgui_support.h"
#include "gui/viewer_panel.h"
#include "render/model_rp.h"
#include "render/render.h"
#include "render/simple_rp.h"
#include "spdlog/spdlog.h"
#include "utility/window.h"


int main(int arch, const char** argv) {
    sid::OpenGLSupport::LoadCore();

    sid::Window window(1280, 720, "Model Viewer");

    if (!window.IsValid()) {
        spdlog::critical("Could not create window");
        sid::OpenGLSupport::TerminateCore();
        return -1;
    }

    if (!sid::OpenGLSupport::LoadExtensions()) {
        spdlog::critical("Could not load opengl extensions");
        sid::OpenGLSupport::TerminateCore();
        return -1;
    }

    sid::ImguiSupport::Init();

    spdlog::info("Application essentials have been initialized");

    sid::OpenGLSupport::EnableVSync();

    // Initialize input event handling
    sid::Controls controls;
    controls.Init(&window);

    // Setup GUI render pass
    auto mv = std::make_shared<viewer::ViewerModel>();
    auto panel = std::make_shared<viewer::ViewerPanel>();
    panel->SetModel(mv);
    auto gui_pass = std::make_shared<sid::GUIRenderPass>();
    gui_pass->Init(window.get());

    mv->model_file = "axe.obj";

    // Must be added last!
    gui_pass->AddPanel(panel);

    // Setup sample render pass
    auto model_rp = std::make_shared<sid::ModelRenderPass>();
    model_rp->Init();

    auto model = sid::make_model();
    model->LoadData(mv->model_file);
    model->SetRotation(0.0, 0.0, 90.0);

    auto second_model = sid::make_model();
    second_model->LoadData(mv->model_file);
    second_model->SetRotation(0.0f, 0.0f, -90.0f);
    second_model->SetPosition(0.0f, 0.1f, 0.0f);

    auto camera = std::make_shared<sid::Camera>();
    camera->SetPosition(0.0, 0.0, -0.5);
    model_rp->SetCamera(camera);

    auto scene = std::make_shared<sid::Scene>();

    scene->AddModel(model);
    scene->AddModel(second_model);

    model_rp->SetScene(scene);

    auto simple_rp = std::make_shared<sid::SimpleRenderPass>();

    // Add render passes to the renderer
    sid::Render render;
    // render.AddPass(simple_rp);
    render.AddPass(model_rp);
    render.AddPass(gui_pass);

    while (window.IsNotClosing()) {
        // Updates, only for UI for now
        controls.UpdateMouseState();
        controls.UpdateMouseCursor();

        // Iterates over all render passes in oreder of addition
        render.Draw();

        window.SwapBuffers();
        window.PollEvents();
    }

    sid::OpenGLSupport::TerminateCore();

    return 0;
}