#include <utility>
#include <optional>
#include <sstream>
#include "tv-show-crawler-runtime/model/CrawlerStep.h"

namespace serio::core {

CrawlerStep::CrawlerStep(std::string type, std::map<std::string, std::string> properties)
    : type(std::move(type)), properties(std::move(properties)) {}

std::string CrawlerStep::getType() const {
    return type;
}

std::optional<std::string> CrawlerStep::getProperty(const std::string &propertyName) const {
    auto propertyValue = properties.find(propertyName);
    if (propertyValue == properties.cend()) {
        return {};
    } else {
        return propertyValue->second;
    }
}

std::string CrawlerStep::getPropertyOrFail(const std::string &propertyName) const {
    auto propertyValue = getProperty(propertyName);
    if (propertyValue) {
        return *propertyValue;
    } else {
        throw CrawlerStepPropertyMissingError(type, propertyName);
    }
}

std::map<std::string, std::string> CrawlerStep::getProperties() const {
    return properties;
}

std::string CrawlerStep::getPropertiesAsString() const {
    std::stringstream props;
    for (const auto& entry: properties) {
        props << entry.first << ": " << entry.second << ", ";
    }
    auto result = props.str();
    return result.substr(0, result.size() - 2);
}

bool CrawlerStep::operator==(const CrawlerStep &rhs) const {
    return type == rhs.type &&
           properties == rhs.properties;
}

bool CrawlerStep::operator!=(const CrawlerStep &rhs) const {
    return !(rhs == *this);
}

CrawlerStepTypeError::CrawlerStepTypeError(const std::string &stepType)
    : invalid_argument("Unknown crawler step type '" + stepType + "'") {}

CrawlerStepPropertyMissingError::CrawlerStepPropertyMissingError(const std::string &stepType,
                                                                 const std::string &missingProperty)
    : std::invalid_argument("Crawler step '" + stepType + "' missing property '" + missingProperty + "'") {}

CrawlerStepExecutionError::CrawlerStepExecutionError(unsigned int stepNumber, const std::exception& error)
    : std::runtime_error("Failed to execute step #" + std::to_string(stepNumber) + ": " + error.what()) {}

}