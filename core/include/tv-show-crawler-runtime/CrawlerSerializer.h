#ifndef SERIO_CRAWLERSERIALIZER_H
#define SERIO_CRAWLERSERIALIZER_H

#include <nlohmann/json.hpp>
#include "tv-show-crawler-runtime/model/TvShowCrawler.h"
#include "tv-show-crawler-runtime/model/CrawlerStepType.h"

namespace serio::core {

class CrawlerSerializer {
public:
    explicit CrawlerSerializer(const std::vector<CrawlerStepType> &crawlerStepTypes);
    [[nodiscard]] std::string serialize(const TvShowCrawler& crawler) const;
    [[nodiscard]] TvShowCrawler deserialize(const std::string& rawCrawler) const;
private:
    const std::vector<CrawlerStepType>& crawlerStepTypes;
    [[nodiscard]] nlohmann::json serialize(const Crawler& crawler) const;
    [[nodiscard]] nlohmann::json serialize(const CrawlerStep& step) const;
    [[nodiscard]] Crawler deserialize(nlohmann::json& crawler, const std::string& crawlerType) const;
    void assertTvShowNameSpecified(nlohmann::json& tvShowCrawler) const;
    void assertTvShowNameNotEmpty(nlohmann::json& tvShowCrawler) const;
    [[nodiscard]] CrawlerStep deserialize(nlohmann::json& crawlerSteps, unsigned int stepIndex) const;
    void assertCrawlerStepTypeSpecified(nlohmann::json& crawlerStep) const;
    void assertCrawlerStepIsValid(nlohmann::json &crawlerStep) const;
    void assertCrawlerStepHasMandatoryProperties(const std::map<std::string, std::string>& properties, const CrawlerStepType& type) const;
};

class TvShowCrawlerNameNotSpecifiedError: public std::runtime_error {
public:
    TvShowCrawlerNameNotSpecifiedError();
};

class TvShowCrawlerNameEmptyError: public std::runtime_error {
public:
    TvShowCrawlerNameEmptyError();
};

class TvShowCrawlerDeserializationError: public std::runtime_error {
public:
    explicit TvShowCrawlerDeserializationError(const std::exception& e);
};

class CrawlerDeserializationError: public std::runtime_error {
public:
    CrawlerDeserializationError(const std::string& crawlerType, const std::runtime_error& e);
};

class CrawlerStepDeserializationError: public std::runtime_error {
public:
    CrawlerStepDeserializationError(unsigned int stepIndex, const std::runtime_error& e);
};

class CrawlerStepTypeNotSpecifiedError: public std::runtime_error {
public:
    CrawlerStepTypeNotSpecifiedError();
};

class UnknownCrawlerStepTypeError: public std::runtime_error {
public:
    explicit UnknownCrawlerStepTypeError(const std::string& type);
};

class MandatoryCrawlerStepPropertiesMissingError: public std::runtime_error {
public:
    MandatoryCrawlerStepPropertiesMissingError(const std::string& type, const std::string& propertiesMissing);
};

}

#endif //SERIO_CRAWLERSERIALIZER_H
