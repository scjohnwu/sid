#pragma once

#include <memory>
#include <string>

#include "glm/mat4x4.hpp"
#include "utility/opengl_support.h"

namespace sid {
class Model {
   public:
    void Draw();
    void LoadData(std::string filename);

    void SetPosition(float x, float y, float z);
    void SetScale(float x, float y, float z);
    void SetRotation(float roll, float pitch, float yaw);

    glm::mat4 GetWorldMat() const;

   private:
    // Mesh utility
    std::vector<float> Normalize(std::vector<float>& vec);
    void Verbose(std::vector<float>& vec);

    // World position
    glm::mat4 m_WorldMat;
    // Calc sources
    float m_PosX{0.0};
    float m_PosY{0.0};
    float m_PosZ{0.0};

    bool m_IsScaleSet{false};
    float m_ScaleX{1.0};
    float m_ScaleY{1.0};
    float m_ScaleZ{1.0};

    float m_Roll{0.0};
    float m_Pitch{0.0};
    float m_Yaw{0.0};
    // Utility
    void UpdateWorldMat();

    VertexArrayPtr m_VAO;
    BufferPtr m_VBO;
    BufferPtr m_EBO;

    VertexAttribPtr m_Position;

    std::size_t m_Indexes{0};
};
using ModelPtr = std::shared_ptr<Model>;

ModelPtr make_model();
}  // namespace sid