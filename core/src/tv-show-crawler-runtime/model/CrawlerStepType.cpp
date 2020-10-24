#include "tv-show-crawler-runtime/model/CrawlerStepType.h"

namespace serio::core {

CrawlerStepType::CrawlerStepType(std::string name, std::string description, std::vector<std::string> mandatoryProperties)
    : name(std::move(name)), description(std::move(description)), mandatoryProperties(std::move(mandatoryProperties)) {}

std::string CrawlerStepType::getName() const {
    return name;
}

std::string CrawlerStepType::getDescription() const {
    return description;
}

std::vector<std::string> CrawlerStepType::getMandatoryProperties() const {
    return mandatoryProperties;
}

bool CrawlerStepType::operator==(const CrawlerStepType &rhs) const {
    return name == rhs.name &&
           description == rhs.description &&
           mandatoryProperties == rhs.mandatoryProperties;
}

bool CrawlerStepType::operator!=(const CrawlerStepType &rhs) const {
    return !(rhs == *this);
}

}