#include <config/ConfigStructure.h>

#include <utility>

namespace serio {
ConfigStructure::ConfigStructure(const nlohmann::json& rootNode) : rootNode(rootNode) {}

ConfigStructure::ConfigStructure(nlohmann::json&& rootNode) : rootNode(rootNode) {}

std::optional<nlohmann::json> ConfigStructure::getParameter(const std::vector<std::string> &path) const {
    auto node = &rootNode;
    for (const auto& segment: path) {
        const auto it = node->find(segment);
        if (it == node->end()) {
            return std::optional<nlohmann::json>();
        } else {
            node = &*it;
        }
    }
    return std::optional<nlohmann::json>(*node);
}

std::vector<ConfigStructure> ConfigStructure::getList(const std::vector<std::string> &path) const {
    std::vector<ConfigStructure> result;
    const auto list = getParameter(path);
    if (list) {
        result.reserve(list->size());
        for (const auto& category: *list) {
            result.emplace_back(category);
        }
    }
    return result;
}
}
