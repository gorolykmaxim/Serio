#ifndef SERIO_HTTPCLIENTMOCK_H
#define SERIO_HTTPCLIENTMOCK_H

#include <gmock/gmock.h>
#include <http-client/HttpClient.h>
#include "NFClientMock.h"
#include "CacheMock.h"

namespace mocks {
class HttpClientMock : public serio::HttpClient {
public:
    inline static NFClientMock CLIENT_STUB;
    inline static CacheMock CACHE_STUB;
    HttpClientMock() : serio::HttpClient(CLIENT_STUB, CACHE_STUB) {}
    MOCK_METHOD(serio::HttpResponse, sendRequest, (const serio::HttpRequest&, const std::chrono::milliseconds&), (override));
};
}

#endif //SERIO_HTTPCLIENTMOCK_H
