#define TINYOBJLOADER_IMPLEMENTATION
#include "model.h"

#include <complex>

#include "glm/gtc/matrix_transform.hpp"
#include "spdlog/spdlog.h"
#include "tiny_obj_loader.h"

namespace sid {
void Model::Draw() {
    m_Position->setBuffer(m_VBO.get(), 0, sizeof(glm::vec3));
    m_VAO->drawElements(gl::GL_TRIANGLES, static_cast<gl::GLsizei>(m_Indexes),
                        gl::GL_UNSIGNED_INT, 0);
}

void Model::LoadData(std::string filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret =
        tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

    if (!warn.empty()) {
        spdlog::warn(warn);
    }

    if (!err.empty()) {
        spdlog::error(err);
    }

    m_VAO = make_vertex_array();
    m_VBO = make_buffer();
    m_EBO = make_buffer();

    m_VAO->bindElementBuffer(m_EBO.get());

    m_Position = m_VAO->binding(0);
    m_Position->setAttribute(0);
    m_Position->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_VAO->enable(0);

    if (shapes.size() == 1) {
        std::vector<gl::GLuint> indexes;

        auto& shape = shapes[0];
        auto& mesh = shape.mesh;

        for (int i = 0; i < mesh.indices.size(); ++i) {
            indexes.push_back(mesh.indices[i].vertex_index);
        }

        auto vertices = Normalize(attrib.vertices);

        auto max_vert = *std::max_element(std::begin(vertices), std::end(vertices));

        Verbose(vertices);

        m_VBO->setData(vertices, gl::GL_STATIC_DRAW);
        m_EBO->setData(indexes, gl::GL_STATIC_DRAW);
        m_Indexes = shape.mesh.indices.size();
    } else {
        spdlog::info("Unsupported number of shapes");
    }
}

void Model::Verbose(std::vector<float>& vec) {
    for (int i = 0; i < vec.size(); i += 3) {
        spdlog::info("Vertex {0} {1} {2}", vec[i], vec[i + 1], vec[i + 2]);
    }
}

std::vector<float> Model::Normalize(std::vector<float>& vec) {
    float norm = 0;
    for (const auto& v : vec) {
        norm += v * v;
    }

    std::vector<float> result;
    if (norm == 0.0) {
        // empty vector
        return result;
    }

    norm = std::sqrt(norm);

    for (const auto& v : vec) {
        result.push_back(v / norm);
    }

    return result;
}

glm::mat4 Model::GetWorldMat() const {
    return m_WorldMat;
}

void Model::SetPosition(float x, float y, float z) {
    m_PosX = x;
    m_PosY = y;
    m_PosZ = z;
    UpdateWorldMat();
}

void Model::SetScale(float x, float y, float z) {
    m_ScaleX = x;
    m_ScaleY = y;
    m_ScaleZ = z;
    UpdateWorldMat();
}

void Model::SetRotation(float roll, float pitch, float yaw) {
    m_Roll = roll;
    m_Pitch = pitch;
    m_Yaw = yaw;
    UpdateWorldMat();
}

void Model::UpdateWorldMat() {
    m_WorldMat = glm::identity<glm::mat4>();
    if (m_IsScaleSet) {
        m_WorldMat = glm::scale(m_WorldMat, glm::vec3{m_ScaleX, m_ScaleY, m_ScaleZ});
    }
    m_WorldMat = glm::translate(m_WorldMat, glm::vec3{m_PosX, m_PosY, m_PosZ});
    m_WorldMat = glm::rotate(m_WorldMat, m_Pitch, glm::vec3{1.0, 0.0, 0.0});
    m_WorldMat = glm::rotate(m_WorldMat, m_Yaw, glm::vec3{0.0, 1.0, 0.0});
    m_WorldMat = glm::rotate(m_WorldMat, m_Roll, glm::vec3{0.0, 0.0, 1.0});
}

ModelPtr make_model() { return std::make_shared<Model>(); }

}  // namespace sid