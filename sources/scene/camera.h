#pragma once

#include <memory>

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace sid {
class Camera {
   public:
    void LookAt(float x, float y, float z);
    void SetPosition(float x, float y, float z);

    glm::mat4 GetViewMat() const;
    glm::mat4 GetProjMat() const;

   private:
    glm::vec3 m_Position{0, 0, 0};
    glm::vec3 m_Target{0, 0, 0};
};
using CameraPtr = std::shared_ptr<Camera>;
}  // namespace sid