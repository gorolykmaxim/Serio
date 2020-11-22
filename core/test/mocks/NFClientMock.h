#ifndef SERIO_NFCLIENTMOCK_H
#define SERIO_NFCLIENTMOCK_H

#include <gmock/gmock.h>
#include <NFHTTP/NFHTTP.h>

MATCHER_P(IsRequest, r, "") {
    size_t size;
    const auto argDataPtr = reinterpret_cast<const char*>(arg->data(size));
    const auto argData = size > 0 ? std::string(argDataPtr) : "";
    const auto rDataPtr = reinterpret_cast<const char*>(r->data(size));
    const auto rData = size > 0 ? std::string(rDataPtr) : "";
    return arg->url() == r->url()
            && arg->method() == r->method()
            && arg->headerMap() == r->headerMap()
            && argData == rData;
}

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
