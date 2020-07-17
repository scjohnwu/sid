#include "viewer_gui.h"
#include "imgui.h"

namespace game {
    void ViewerGUI::Draw() {
        ImGui::ShowDemoWindow(&m_ShowDemo);
    }
}