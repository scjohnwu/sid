#include "utility/window.h"
#include "render/render.h"

#include "gui/imgui_support.h"
#include "gui/viewer_gui.h"
#include "gui/controls.h"
#include "gui/gui_render_pass.h"

#include "render/model_rp.h"
#include "spdlog/spdlog.h"

int main(int arch, const char** argv) {
    sid::OpenGLSupport::LoadCore();

    sid::Window window(1280, 720, "Shooting in the dark");

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
    auto layout = std::make_shared<game::ViewerGUI>();
    auto gui_pass = std::make_shared<sid::GUIRenderPass>();
    gui_pass->Init(window.get());

    // Must be added last!
    gui_pass->SetLayout(layout);

    // Setup sample render pass
    auto model_rp = std::make_shared<sid::ModelRenderPass>();
    model_rp->Init();

    auto model = sid::make_model();
    model->LoadData("axe.obj");
    model_rp->SetModel(model);

    // Add render passes to the renderer
    sid::Render render;
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