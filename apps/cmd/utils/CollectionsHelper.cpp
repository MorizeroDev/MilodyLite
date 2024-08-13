
#include "CollectionsHelper.h"

namespace CollectionsHelper {
std::string join(std::vector<std::string> const& args) {
    std::stringstream ss;
    for (auto& item: args) {
        ss << item << " ";
    }
    return ss.str();
}
} // namespace CollectionsHelper
