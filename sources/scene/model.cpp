#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "spdlog/spdlog.h"

#include "model.h"

namespace sid {
void Model::Draw() {}

ModelPtr load_model(std::string filename) {
    ModelPtr result = std::make_shared<Model>();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

    if (!warn.empty()) {
        spdlog::warn(warn);
    }

    if (!err.empty()) {
        spdlog::error(err);
    }

    return result;
}
}  // namespace sid