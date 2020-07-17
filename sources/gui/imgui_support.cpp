#include "imgui_support.h"

#include "imgui.h"

namespace sid {
void ImguiSupport::Init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
}
}  // namespace sid