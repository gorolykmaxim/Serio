#ifndef SERIO_NFCLIENTMOCK_H
#define SERIO_NFCLIENTMOCK_H

#include <gmock/gmock.h>
#include <NFHTTP/NFHTTP.h>

namespace mocks {
class NFClientMock : nativeformat::http::Client {
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
