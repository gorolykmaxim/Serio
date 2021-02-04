#ifndef SERIO_CONFIGMOCK_H
#define SERIO_CONFIGMOCK_H

#include <gmock/gmock.h>
#include <config/Config.h>
#include "HttpClientMock.h"

namespace mocks {
class ConfigMock : public serio::Config {
public:
    inline static HttpClientMock CLIENT_STUB;
    ConfigMock() : serio::Config(Database::STUB, CLIENT_STUB) {}
    MOCK_METHOD(void, setSourceUrl, (const std::string&), (override));
    MOCK_METHOD((std::optional<std::string>), getSourceUrl, (), (override));
    MOCK_METHOD(void, setProperty, (const std::string&, const std::string&), (override));
    MOCK_METHOD((std::optional<std::string>), getProperty, (const std::string&), (override));
    MOCK_METHOD(serio::HttpClientConfig, getHttpClientConfig, (), (override));
    MOCK_METHOD((std::vector<serio::TvShowCrawlerConfig>), getTvShowCrawlerConfigs, (), (override));
    MOCK_METHOD((std::vector<serio::SearchCrawlerConfig>), getSearchCrawlerConfigs, (), (override));
    MOCK_METHOD((std::vector<serio::CategoryCrawlerConfig>), getCategoryCrawlerConfigs, (), (override));
    MOCK_METHOD((std::vector<serio::SuggestionsCrawlerConfig>), getSuggestionsCrawlerConfigs, (), (override));
};
}

#endif //SERIO_CONFIGMOCK_H
