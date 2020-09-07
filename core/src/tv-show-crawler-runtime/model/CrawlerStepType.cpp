#include "tv-show-crawler-runtime/model/CrawlerStepType.h"

serio::core::CrawlerStepType::CrawlerStepType(std::string name, std::string description, std::vector<std::string> mandatoryProperties)
    : name(std::move(name)), description(std::move(description)), mandatoryProperties(std::move(mandatoryProperties)) {}

std::string serio::core::CrawlerStepType::getName() const {
    return name;
}

std::string serio::core::CrawlerStepType::getDescription() const {
    return description;
}

std::vector<std::string> serio::core::CrawlerStepType::getMandatoryProperties() const {
    return mandatoryProperties;
}

bool serio::core::CrawlerStepType::operator==(const serio::core::CrawlerStepType &rhs) const {
    return name == rhs.name &&
           description == rhs.description &&
           mandatoryProperties == rhs.mandatoryProperties;
}

bool serio::core::CrawlerStepType::operator!=(const serio::core::CrawlerStepType &rhs) const {
    return !(rhs == *this);
}
