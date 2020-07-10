#pragma once

#include <memory>

namespace sid {
class RenderPass {
   public:
    virtual void Draw() = 0;

    virtual ~RenderPass() = default;
};
using RenderPassPtr = std::shared_ptr<RenderPass>;

}  // namespace sid