#ifndef SERIO_HTTP_MOCK_H
#define SERIO_HTTP_MOCK_H

#include <gmock/gmock.h>
#include <NFHTTP/Client.h>
#include <NFHTTP/ResponseImplementation.h>

MATCHER_P(is_request, r, "") {
    size_t size;
    const auto arg_data_ptr = reinterpret_cast<const char*>(arg->data(size));
    const auto arg_data = size > 0 ? std::string(arg_data_ptr) : "";
    const auto r_data_ptr = reinterpret_cast<const char*>(r->data(size));
    const auto r_data = size > 0 ? std::string(r_data_ptr) : "";
    return arg->url() == r->url()
            && arg->method() == r->method()
            && arg->headerMap() == r->headerMap()
            && arg_data == r_data;
}

class nf_client_mock: public nativeformat::http::Client {
public:
    MOCK_METHOD((std::shared_ptr<nativeformat::http::RequestToken>),
            performRequest,
    (const std::shared_ptr<nativeformat::http::Request>&, std::function<void(const std::shared_ptr<nativeformat::http::Response>&)>),
    (override));

    void mock_response(const std::string& url, const std::string& res_body,
                       nativeformat::http::StatusCode code = nativeformat::http::StatusCodeOK) {
        const auto req = nativeformat::http::createRequest(url, {});
        const auto res = std::make_shared<nativeformat::http::ResponseImplementation>(req,
                                                                                      reinterpret_cast<const unsigned char*>(res_body.c_str()),
                                                                                      res_body.size(),
                                                                                      code, false);
        const auto invoke_callback = ::testing::InvokeArgument<1>(res);
        const auto return_null = ::testing::Return(std::shared_ptr<nativeformat::http::RequestToken>(nullptr));
        ON_CALL(*this, performRequest(is_request(req), ::testing::_)).WillByDefault(::testing::DoAll(invoke_callback, return_null));
    }

    void mock_no_internet() {
        const auto req = nativeformat::http::createRequest("", {});
        const auto res = std::make_shared<nativeformat::http::ResponseImplementation>(req,
                                                                                      nullptr,
                                                                                      0,
                                                                                      nativeformat::http::StatusCodeInvalid,
                                                                                      false);
        const auto invoke_callback = ::testing::InvokeArgument<1>(res);
        const auto return_null = ::testing::Return(std::shared_ptr<nativeformat::http::RequestToken>(nullptr));
        ON_CALL(*this, performRequest(testing::_, ::testing::_)).WillByDefault(::testing::DoAll(invoke_callback, return_null));
    }
};

#endif //SERIO_HTTP_MOCK_H
