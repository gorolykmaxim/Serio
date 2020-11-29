#include "HardcodedConfig.h"

#include <utility>

namespace crdb {
HardcodedConfig::HardcodedConfig(std::string  userAgent, SQLite::Database &database, serio::CachingHttpClient &httpClient)
    : Config(database, httpClient), userAgent(std::move(userAgent)) {}

serio::HttpClientConfig HardcodedConfig::getHttpClientConfig() {
    return serio::HttpClientConfig{{userAgent}};
}
}