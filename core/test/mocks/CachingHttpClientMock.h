#ifndef SERIO_CACHINGHTTPCLIENTMOCK_H
#define SERIO_CACHINGHTTPCLIENTMOCK_H

#include <gmock/gmock.h>
#include <caching-http-client/CachingHttpClient.h>
#include "NFClientMock.h"
#include "CacheMock.h"

namespace mocks {
class CachingHttpClientMock : public serio::CachingHttpClient {
public:
    inline static NFClientMock CLIENT_STUB;
    inline static CacheMock CACHE_STUB;
    CachingHttpClientMock() : serio::CachingHttpClient(CLIENT_STUB, CACHE_STUB) {}
    MOCK_METHOD(serio::HttpResponse, sendRequest, (const serio::HttpRequest&, const std::chrono::milliseconds&), (override));
};
}

#endif //SERIO_CACHINGHTTPCLIENTMOCK_H
