#include <config/ConfigStructure.h>

namespace serio {
ConfigStructure::ConfigStructure(nlohmann::json&& rootNode) : rootNode(rootNode) {}

std::optional<nlohmann::json> ConfigStructure::getParameter(const std::vector<std::string> &path) const {
    auto node = rootNode;
    for (const auto& segment: path) {
        const auto it = node.find(segment);
        if (it == node.end()) {
            return std::optional<nlohmann::json>();
        } else {
            node = *it;
        }
    }
    return node;
}
}
