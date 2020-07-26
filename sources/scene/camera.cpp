#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace sid {
void Camera::LookAt(float x, float y, float z) { m_Target = glm::vec3{x, y, z}; }

void Camera::SetPosition(float x, float y, float z) { m_Position = glm::vec3{x, y, z}; }

glm::mat4 Camera::GetViewMat() const {
    static glm::vec3 s_up{0.0f, 1.0f, 0.0f};
    return glm::lookAt(m_Position, m_Target, s_up);
}

glm::mat4 Camera::GetProjMat() const {
    return glm::perspective(45.0f, (float)1280 / (float)720, 0.01f, 1000.0f);
}

}  // namespace sid