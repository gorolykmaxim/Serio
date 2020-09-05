#ifndef SERIO_HTTPCLIENTMOCK_H
#define SERIO_HTTPCLIENTMOCK_H

#include <gmock/gmock.h>
#include <http-client/HttpClient.h>

class HttpClientMock : public serio::core::HttpClient {
public:
    MOCK_METHOD((std::future<std::vector<std::string>>), fetchContentFromLinks, (const std::vector<std::string>&), (override));
};

#endif //SERIO_HTTPCLIENTMOCK_H
