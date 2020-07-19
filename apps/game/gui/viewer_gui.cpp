#include "viewer_gui.h"
#include "imgui.h"

namespace game {
    void ViewerGUI::Draw() {
        using namespace ImGui;
        ImGui::ShowDemoWindow();
    }
}
