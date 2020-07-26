#include "scene.h"

namespace sid {
    void Scene::AddModel(ModelPtr model) {
        m_Models.push_back(model);
    }

    std::vector<ModelPtr> Scene::GetRenderModels() {
        return m_Models;
    }
}