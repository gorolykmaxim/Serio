#include "tv-show-crawler-runtime/model/CrawlerStepType.h"

serio::core::CrawlerStepType::CrawlerStepType(std::string name, std::vector<std::string> mandatoryProperties)
    : name(std::move(name)), mandatoryProperties(std::move(mandatoryProperties)) {}

std::string serio::core::CrawlerStepType::getName() const {
    return name;
}

std::vector<std::string> serio::core::CrawlerStepType::getMandatoryProperties() const {
    return mandatoryProperties;
}

bool serio::core::CrawlerStepType::operator==(const serio::core::CrawlerStepType &rhs) const {
    return name == rhs.name &&
           mandatoryProperties == rhs.mandatoryProperties;
}

bool serio::core::CrawlerStepType::operator!=(const serio::core::CrawlerStepType &rhs) const {
    return !(rhs == *this);
}
