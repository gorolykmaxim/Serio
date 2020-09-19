#include "tv-show-crawler-runtime/CrawlerSerializer.h"
#include <algorithm>
#include <sstream>

serio::core::CrawlerSerializer::CrawlerSerializer(const std::vector<serio::core::CrawlerStepType> &crawlerStepTypes)
        : crawlerStepTypes(crawlerStepTypes) {}

std::string serio::core::CrawlerSerializer::serialize(const TvShowCrawler& crawler) const {
    nlohmann::json serializedCrawler;
    serializedCrawler["showName"] = crawler.getTvShowName();
    serializedCrawler["episodeVideoCrawler"] = serialize(crawler.getCrawler(CrawlerType::episodeVideoCrawler));
    serializedCrawler["thumbnailCrawler"] = serialize(crawler.getCrawler(CrawlerType::thumbnailCrawler));
    serializedCrawler["episodeNameCrawler"] = serialize(crawler.getCrawler(CrawlerType::episodeNameCrawler));
    return serializedCrawler.dump();
}

nlohmann::json serio::core::CrawlerSerializer::serialize(const serio::core::Crawler &crawler) const {
    nlohmann::json serializedSteps = std::vector<nlohmann::json>();
    for (const CrawlerStep& step: crawler.getSteps()) {
        serializedSteps.push_back(serialize(step));
    }
    return {{"steps", serializedSteps}};
}

nlohmann::json serio::core::CrawlerSerializer::serialize(const CrawlerStep &step) const {
    nlohmann::json serializedStep = {{"type", step.getType()}};
    for (const auto& [k, v] : step.getProperties()) {
        serializedStep[k] = v;
    }
    return serializedStep;
}

serio::core::TvShowCrawler serio::core::CrawlerSerializer::deserialize(const std::string &rawCrawler) const {
    try {
        nlohmann::json deserializedCrawler = nlohmann::json::parse(rawCrawler);
        assertTvShowNameSpecified(deserializedCrawler);
        assertTvShowNameNotEmpty(deserializedCrawler);
        return serio::core::TvShowCrawler(
                deserializedCrawler["showName"],
                core::Crawler(deserialize(deserializedCrawler, "episodeVideoCrawler")),
                core::Crawler(deserialize(deserializedCrawler, "thumbnailCrawler")),
                core::Crawler(deserialize(deserializedCrawler, "episodeNameCrawler")));
    } catch (std::runtime_error& e) {
        throw TvShowCrawlerDeserializationError(e);
    }
}

serio::core::Crawler serio::core::CrawlerSerializer::deserialize(nlohmann::json &crawler, const std::string& crawlerType) const {
    try {
        nlohmann::json& deserializedSteps = crawler[crawlerType]["steps"];
        std::vector<serio::core::CrawlerStep> steps;
        for (int i = 0; i < deserializedSteps.size(); i++) {
            steps.push_back(deserialize(deserializedSteps, i));
        }
        return serio::core::Crawler(steps);
    } catch (std::runtime_error& e) {
        throw CrawlerDeserializationError(crawlerType, e);
    }
}

void serio::core::CrawlerSerializer::assertTvShowNameSpecified(nlohmann::json &tvShowCrawler) const {
    if (tvShowCrawler["showName"].is_null()) {
        throw TvShowCrawlerNameNotSpecifiedError();
    }
}

void serio::core::CrawlerSerializer::assertTvShowNameNotEmpty(nlohmann::json &tvShowCrawler) const {
    if (tvShowCrawler["showName"] == "") {
        throw TvShowCrawlerNameEmptyError();
    }
}

serio::core::CrawlerStep serio::core::CrawlerSerializer::deserialize(nlohmann::json &crawlerSteps, unsigned int stepIndex) const {
    try {
        nlohmann::json& crawlerStep = crawlerSteps[stepIndex];
        assertCrawlerStepTypeSpecified(crawlerStep);
        assertCrawlerStepIsValid(crawlerStep);
        std::map<std::string, std::string> properties = crawlerStep;
        properties.erase("type");
        return CrawlerStep(crawlerStep["type"], properties);
    } catch (std::runtime_error& e) {
        throw CrawlerStepDeserializationError(stepIndex + 1, e);
    }
}

void serio::core::CrawlerSerializer::assertCrawlerStepTypeSpecified(nlohmann::json &crawlerStep) const {
    if (!crawlerStep.contains("type")) {
        throw CrawlerStepTypeNotSpecifiedError();
    }
}

void serio::core::CrawlerSerializer::assertCrawlerStepIsValid(nlohmann::json &crawlerStep) const {
    std::string type = crawlerStep["type"];
    auto it = std::find_if(crawlerStepTypes.cbegin(), crawlerStepTypes.cend(), [type] (const CrawlerStepType& crawlerStepType) { return crawlerStepType.getName() == type; });
    if (it == crawlerStepTypes.cend()) {
        throw UnknownCrawlerStepTypeError(type);
    }
    assertCrawlerStepHasMandatoryProperties(crawlerStep, *it);
}

void serio::core::CrawlerSerializer::assertCrawlerStepHasMandatoryProperties(const std::map<std::string, std::string> &properties,
                                                                             const serio::core::CrawlerStepType &type) const {
    std::stringstream missingProperties;
    for (const std::string& mandatoryProperty: type.getMandatoryProperties()) {
        if (properties.find(mandatoryProperty) == properties.cend()) {
            missingProperties << "\"" << mandatoryProperty << "\" ";
        }
    }
    std::string missingPropertiesString = missingProperties.str();
    if (!missingPropertiesString.empty()) {
        missingPropertiesString.resize(missingPropertiesString.size() - 1);
        throw MandatoryCrawlerStepPropertiesMissingError(type.getName(), missingPropertiesString);
    }
}

serio::core::TvShowCrawlerNameNotSpecifiedError::TvShowCrawlerNameNotSpecifiedError()
    : std::runtime_error("TV show name not specified") {}

serio::core::TvShowCrawlerNameEmptyError::TvShowCrawlerNameEmptyError()
    : std::runtime_error("TV show name is empty") {}

serio::core::TvShowCrawlerDeserializationError::TvShowCrawlerDeserializationError(const std::runtime_error &e)
    : runtime_error(std::string("Failed to deserialize TV show crawler: ") + e.what()) {}

serio::core::CrawlerDeserializationError::CrawlerDeserializationError(const std::string& crawlerType,
                                                                      const std::runtime_error &e)
    : runtime_error(std::string("Failed to deserialize \"") + crawlerType + "\": " + e.what()) {}

serio::core::CrawlerStepDeserializationError::CrawlerStepDeserializationError(unsigned int stepIndex,
                                                                              const std::runtime_error &e)
    : std::runtime_error("Failed to deserialize crawler step #" + std::to_string(stepIndex) + ": " + e.what()) {}

serio::core::CrawlerStepTypeNotSpecifiedError::CrawlerStepTypeNotSpecifiedError()
    : std::runtime_error("Crawler step type not specified") {}

serio::core::UnknownCrawlerStepTypeError::UnknownCrawlerStepTypeError(const std::string &type)
    : runtime_error("Unknown crawler step type \"" + type + "\"") {}

serio::core::MandatoryCrawlerStepPropertiesMissingError::MandatoryCrawlerStepPropertiesMissingError(const std::string &type,
                                                                                                    const std::string &propertiesMissing)
    : std::runtime_error("Crawler step with type \"" + type + "\" missing following mandatory properties: " + propertiesMissing) {}
