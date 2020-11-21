#ifndef SERIO_CONFIGSTRUCTURE_H
#define SERIO_CONFIGSTRUCTURE_H

#include <nlohmann/json.hpp>
#include <optional>

namespace serio {
class ConfigStructure {
public:
    explicit ConfigStructure(const nlohmann::json& rootNode);
    explicit ConfigStructure(nlohmann::json&& rootNode);
    [[nodiscard]] std::optional<nlohmann::json> getParameter(const std::vector<std::string> &path) const;
    [[nodiscard]] std::vector<ConfigStructure> getPlatforms() const;
private:
    nlohmann::json rootNode;
};
}

#endif //SERIO_CONFIGSTRUCTURE_H
