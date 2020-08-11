#pragma once

#include "gui/gui_panel.h"
#include "viewer_model.h"

namespace viewer {
class ViewerPanel : public sid::GUIPanel {
   public:
    void Draw() override;
    const char* GetName() const override;
    void SetModel(ViewerModelPtr model);
   private:
    bool m_IsShowing{true};
    ViewerModelPtr m_Model;
};
}  // namespace game