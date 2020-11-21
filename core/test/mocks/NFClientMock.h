#ifndef SERIO_NFCLIENTMOCK_H
#define SERIO_NFCLIENTMOCK_H

#include <gmock/gmock.h>
#include <NFHTTP/NFHTTP.h>

MATCHER_P(IsRequest, r, "") { return arg->url() == r->url(); }

namespace mocks {
class NFClientMock : public nativeformat::http::Client {
public:
    MOCK_METHOD(
            (std::shared_ptr<nativeformat::http::RequestToken>),
            performRequest,
            (const std::shared_ptr<nativeformat::http::Request>&, std::function<void(const std::shared_ptr<nativeformat::http::Response>&)>),
            (override));
    MOCK_METHOD(
            (const std::shared_ptr<nativeformat::http::Response>),
            performRequestSynchronously,
            (const std::shared_ptr<nativeformat::http::Request>&),
            (override));
};
}

#endif //SERIO_NFCLIENTMOCK_H
