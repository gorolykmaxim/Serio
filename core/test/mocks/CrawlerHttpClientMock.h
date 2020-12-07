#ifndef SERIO_CRAWLERHTTPCLIENTMOCK_H
#define SERIO_CRAWLERHTTPCLIENTMOCK_H

#include <gmock/gmock.h>
#include <crawler-http-client/CrawlerHttpClient.h>
#include "ConfigMock.h"

namespace mocks {
class CrawlerHttpClientMock : public serio::CrawlerHttpClient {
public:
    inline static ConfigMock CONFIG_STUB;
    inline static CachingHttpClientMock CLIENT_STUB;
    CrawlerHttpClientMock() : serio::CrawlerHttpClient(CONFIG_STUB, CLIENT_STUB) {}
    MOCK_METHOD((serio::HttpResponse), sendRequest, (serio::HttpRequest, std::chrono::milliseconds), (override));
};
}

#endif //SERIO_CRAWLERHTTPCLIENTMOCK_H
