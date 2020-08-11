#include "viewer_panel.h"

#include "imgui.h"


namespace viewer {
void ViewerPanel::Draw() {
    using namespace ImGui;
    if (m_Model) {
        Text("Filename: %s", m_Model->model_file.c_str());
        Button("File...", ImVec2{80, 40});
    }
}

void ViewerPanel::SetModel(ViewerModelPtr model) {
    m_Model = model;
}

const char* ViewerPanel::GetName() const { return "Viewer"; }
}  // namespace viewer
