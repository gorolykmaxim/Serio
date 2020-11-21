#include <config/ConfigStructure.h>

#include <utility>

namespace serio {
ConfigStructure::ConfigStructure(const nlohmann::json& rootNode) : rootNode(rootNode) {}

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

std::vector<ConfigStructure> ConfigStructure::getPlatforms() const {
    std::vector<ConfigStructure> result;
    const auto platforms = rootNode.find("platforms");
    if (platforms != rootNode.cend()) {
        result.reserve(platforms->size());
        for (const auto& platform: *platforms) {
            result.emplace_back(platform);
        }
    }
    return result;
}
}
