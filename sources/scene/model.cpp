#define TINYOBJLOADER_IMPLEMENTATION
#include "model.h"

#include "spdlog/spdlog.h"
#include "tiny_obj_loader.h"

namespace sid {
void Model::Draw() {
    m_Position->setBuffer(m_VBO.get(), 0, sizeof(glm::vec3));
    m_VAO->drawElements(gl::GL_TRIANGLES, m_Indexes, gl::GL_UNSIGNED_INT, 0);
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

        m_VBO->setData(attrib.vertices, gl::GL_STATIC_DRAW);
        m_EBO->setData(indexes, gl::GL_STATIC_DRAW);
        m_Indexes = shape.mesh.indices.size();
    } else {
        spdlog::info("Unsupported number of shapes");
    }
}

ModelPtr make_model() { return std::make_shared<Model>(); }

}  // namespace sid