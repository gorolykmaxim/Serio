#include "tv-show-crawler-runtime/CrawlerSerializer.h"
#include <algorithm>
#include <sstream>

namespace serio::core {

CrawlerSerializer::CrawlerSerializer(const std::vector<CrawlerStepType> &crawlerStepTypes)
    : crawlerStepTypes(crawlerStepTypes) {}

std::string CrawlerSerializer::serialize(const TvShowCrawler& crawler) const {
    nlohmann::json serializedCrawler;
    serializedCrawler["showName"] = crawler.getTvShowName();
    serializedCrawler["episodeVideoCrawler"] = serialize(crawler.getCrawler(CrawlerType::episodeVideoCrawler));
    serializedCrawler["thumbnailCrawler"] = serialize(crawler.getCrawler(CrawlerType::thumbnailCrawler));
    serializedCrawler["episodeNameCrawler"] = serialize(crawler.getCrawler(CrawlerType::episodeNameCrawler));
    return serializedCrawler.dump();
}

nlohmann::json CrawlerSerializer::serialize(const Crawler &crawler) const {
    auto serializedSteps = std::vector<nlohmann::json>();
    for (const CrawlerStep& step: crawler.getSteps()) {
        serializedSteps.push_back(serialize(step));
    }
    return {{"steps", serializedSteps}};
}

nlohmann::json CrawlerSerializer::serialize(const CrawlerStep &step) const {
    nlohmann::json serializedStep = {{"type", step.getType()}};
    for (const auto& [k, v] : step.getProperties()) {
        serializedStep[k] = v;
    }
    return serializedStep;
}

TvShowCrawler CrawlerSerializer::deserialize(const std::string &rawCrawler) const {
    try {
        auto deserializedCrawler = nlohmann::json::parse(rawCrawler);
        assertTvShowNameSpecified(deserializedCrawler);
        assertTvShowNameNotEmpty(deserializedCrawler);
        return TvShowCrawler(
                deserializedCrawler["showName"],
                Crawler(deserialize(deserializedCrawler, "episodeVideoCrawler")),
                Crawler(deserialize(deserializedCrawler, "thumbnailCrawler")),
                Crawler(deserialize(deserializedCrawler, "episodeNameCrawler")));
    } catch (std::exception& e) {
        throw TvShowCrawlerDeserializationError(e);
    }
}

Crawler CrawlerSerializer::deserialize(nlohmann::json &crawler, const std::string& crawlerType) const {
    try {
        auto& deserializedSteps = crawler[crawlerType]["steps"];
        std::vector<CrawlerStep> steps;
        steps.reserve(deserializedSteps.size());
        for (int i = 0; i < deserializedSteps.size(); i++) {
            steps.push_back(deserialize(deserializedSteps, i));
        }
        return Crawler(steps);
    } catch (std::runtime_error& e) {
        throw CrawlerDeserializationError(crawlerType, e);
    }
}

void CrawlerSerializer::assertTvShowNameSpecified(nlohmann::json &tvShowCrawler) const {
    if (tvShowCrawler["showName"].is_null()) {
        throw TvShowCrawlerNameNotSpecifiedError();
    }
}

void CrawlerSerializer::assertTvShowNameNotEmpty(nlohmann::json &tvShowCrawler) const {
    if (tvShowCrawler["showName"] == "") {
        throw TvShowCrawlerNameEmptyError();
    }
}

CrawlerStep CrawlerSerializer::deserialize(nlohmann::json &crawlerSteps, unsigned int stepIndex) const {
    try {
        auto& crawlerStep = crawlerSteps[stepIndex];
        assertCrawlerStepTypeSpecified(crawlerStep);
        assertCrawlerStepIsValid(crawlerStep);
        auto properties = crawlerStep;
        properties.erase("type");
        return CrawlerStep(crawlerStep["type"], properties);
    } catch (std::runtime_error& e) {
        throw CrawlerStepDeserializationError(stepIndex + 1, e);
    }
}

void CrawlerSerializer::assertCrawlerStepTypeSpecified(nlohmann::json &crawlerStep) const {
    if (!crawlerStep.contains("type")) {
        throw CrawlerStepTypeNotSpecifiedError();
    }
}

void CrawlerSerializer::assertCrawlerStepIsValid(nlohmann::json &crawlerStep) const {
    auto type = crawlerStep["type"];
    auto it = std::find_if(crawlerStepTypes.cbegin(), crawlerStepTypes.cend(),
                           [type] (const CrawlerStepType& crawlerStepType) { return crawlerStepType.getName() == type; });
    if (it == crawlerStepTypes.cend()) {
        throw UnknownCrawlerStepTypeError(type);
    }
    assertCrawlerStepHasMandatoryProperties(crawlerStep, *it);
}

void CrawlerSerializer::assertCrawlerStepHasMandatoryProperties(const std::map<std::string, std::string> &properties,
                                                                const CrawlerStepType &type) const {
    std::stringstream missingProperties;
    for (const auto& mandatoryProperty: type.getMandatoryProperties()) {
        if (properties.find(mandatoryProperty) == properties.cend()) {
            missingProperties << "\"" << mandatoryProperty << "\" ";
        }
    }
    auto missingPropertiesString = missingProperties.str();
    if (!missingPropertiesString.empty()) {
        missingPropertiesString.resize(missingPropertiesString.size() - 1);
        throw MandatoryCrawlerStepPropertiesMissingError(type.getName(), missingPropertiesString);
    }
}

TvShowCrawlerNameNotSpecifiedError::TvShowCrawlerNameNotSpecifiedError()
    : std::runtime_error("TV show name not specified") {}

TvShowCrawlerNameEmptyError::TvShowCrawlerNameEmptyError()
    : std::runtime_error("TV show name is empty") {}

TvShowCrawlerDeserializationError::TvShowCrawlerDeserializationError(const std::exception &e)
    : std::runtime_error(std::string("Failed to deserialize TV show crawler: ") + e.what()) {}

CrawlerDeserializationError::CrawlerDeserializationError(const std::string& crawlerType, const std::runtime_error &e)
    : std::runtime_error(std::string("Failed to deserialize \"") + crawlerType + "\": " + e.what()) {}

CrawlerStepDeserializationError::CrawlerStepDeserializationError(unsigned int stepIndex, const std::runtime_error &e)
    : std::runtime_error("Failed to deserialize crawler step #" + std::to_string(stepIndex) + ": " + e.what()) {}

CrawlerStepTypeNotSpecifiedError::CrawlerStepTypeNotSpecifiedError()
    : std::runtime_error("Crawler step type not specified") {}

UnknownCrawlerStepTypeError::UnknownCrawlerStepTypeError(const std::string &type)
    : runtime_error("Unknown crawler step type \"" + type + "\"") {}

MandatoryCrawlerStepPropertiesMissingError::MandatoryCrawlerStepPropertiesMissingError(const std::string &type,
                                                                                       const std::string &propertiesMissing)
    : std::runtime_error("Crawler step with type \"" + type + "\" missing following mandatory properties: " + propertiesMissing) {}

}