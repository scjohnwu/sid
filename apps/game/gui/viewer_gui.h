#pragma once

#include "gui/gui_layout.h"

namespace game {
    class ViewerGUI : public sid::GUILayout {
    public:
        void Draw() override;
    private:
        bool m_ShowDemo {true};
    };
}