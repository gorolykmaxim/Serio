#ifndef SERIO_CRAWLERHTTPCLIENT_H
#define SERIO_CRAWLERHTTPCLIENT_H

#include <config/Config.h>

namespace serio {
class CrawlerHttpClient {
public:
    CrawlerHttpClient(Config& config, CachingHttpClient& client);
    virtual std::future<std::string> sendRequest(HttpRequest request, std::chrono::milliseconds cacheTtl);
private:
    Config& config;
    CachingHttpClient& client;
};
}

#endif //SERIO_CRAWLERHTTPCLIENT_H
