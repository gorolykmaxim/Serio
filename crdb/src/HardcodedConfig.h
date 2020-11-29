#ifndef SERIO_HARDCODEDCONFIG_H
#define SERIO_HARDCODEDCONFIG_H

#include <config/Config.h>

namespace crdb {
class HardcodedConfig : public serio::Config {
public:
    HardcodedConfig(std::string  userAgent, SQLite::Database &database, serio::CachingHttpClient &httpClient);
    serio::HttpClientConfig getHttpClientConfig() override;
private:
    std::string userAgent;
};
}

#endif //SERIO_HARDCODEDCONFIG_H
