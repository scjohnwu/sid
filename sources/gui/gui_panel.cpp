#include "gui_panel.h"

namespace sid {
bool* GUIPanel::IsVisible() { return &m_IsVisible; }

const char* GUIPanel::GetName() const { return "default panel"; }

int GUIPanel::GetFlags() const { return 0; }
}  // namespace sid