#pragma once

#include <memory>

namespace sid {
class GUILayout {
   public:
    virtual void Draw() = 0;
    virtual ~GUILayout() = default;
};
using GUILayoutPtr = std::shared_ptr<GUILayout>;

}  // namespace sid