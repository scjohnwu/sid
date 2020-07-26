#pragma once

#include "model.h"

namespace sid {
class Scene {
   public:
    void AddModel(ModelPtr model);

    std::vector<ModelPtr> GetRenderModels();
   private:
    std::vector<ModelPtr> m_Models;
};

using ScenePtr = std::shared_ptr<Scene>;
}  // namespace sid