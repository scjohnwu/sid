#include <memory>
#include <string>

namespace sid {
class Model {
   public:
    void Draw();
};
using ModelPtr = std::shared_ptr<Model>;

ModelPtr load_model(std::string filename);
}  // namespace sid