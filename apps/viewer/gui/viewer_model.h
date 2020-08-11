#pragma once

#include <memory>
#include <string>

namespace viewer {
struct ViewerModel {
    std::string model_file;
};
using ViewerModelPtr = std::shared_ptr<ViewerModel>;
}  // namespace viewer