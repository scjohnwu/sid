#pragma once

#include <memory>

namespace sid {
class GUIPanel {
   public:
    virtual void Draw() = 0;
    virtual ~GUIPanel() = default;
    bool* IsVisible();
    virtual int GetFlags() const;
    virtual const char* GetName() const;

   private:
    bool m_IsVisible{true};
};
using GUIPanelPtr = std::shared_ptr<GUIPanel>;

}  // namespace sid