#pragma once

#include <vector>

#include "render_pass.h"
#include "scene.h"

namespace sid {
class Render {
   public:
    void Draw();
    void AddPass(RenderPassPtr render_pass);

   private:
    ScenePtr m_CurrentScene;

    std::vector<RenderPassPtr> m_Passes;
};
}  // namespace sid