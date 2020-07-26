#pragma once

#include "gui/gui_layout.h"

namespace game {
class ViewerGUI : public sid::GUILayout {
   public:
    void Draw() override;

   private:
    bool m_IsShowing{true};
};
}  // namespace game