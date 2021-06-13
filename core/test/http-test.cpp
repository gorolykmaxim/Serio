#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <NFHTTP/Client.h>
#include <NFHTTP/ResponseImplementation.h>
#include <SQLiteCpp/Database.h>
#include <http.h>
#include <iconvlite.h>
#include <task.h>

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

    void mock_response(const std::shared_ptr<nativeformat::http::Request>& req,
                       const std::shared_ptr<nativeformat::http::Response>& res) {
        const auto invoke_callback = ::testing::InvokeArgument<1>(res);
        const auto return_null = ::testing::Return(std::shared_ptr<nativeformat::http::RequestToken>(nullptr));
        ON_CALL(*this, performRequest(is_request(req), ::testing::_)).WillByDefault(::testing::DoAll(invoke_callback, return_null));
    }

    std::shared_ptr<nativeformat::http::Request> to_nf_request(const http_request& req,
                                                               const std::optional<std::string>& user_agent = {}) {
        auto headers = req.headers;
        if (user_agent) {
            headers[USER_AGENT_HTTP_HEADER] = *user_agent;
        }
        auto nf_req = nativeformat::http::createRequest(req.url, headers);
        if (!req.method.empty()) {
            nf_req->setMethod(req.method);
        }
        nf_req->setData(reinterpret_cast<const unsigned char*>(req.body.c_str()), req.body.size());
        return nf_req;
    }

    void mock_response(const http_request& req, const std::string& res,
                       nativeformat::http::StatusCode code = nativeformat::http::StatusCode::StatusCodeOK,
                       const std::optional<std::string>& content_type = {}) {
        const auto nf_req = to_nf_request(req, std::optional<std::string>());
        auto nf_res = std::make_shared<nativeformat::http::ResponseImplementation>(
                nf_req,
                reinterpret_cast<const unsigned char*>(res.c_str()),
                res.size(),
                code,
                false);
        if (content_type) {
            nf_res->operator[](CONTENT_TYPE_HTTP_HEADER) = *content_type;
        }
        mock_response(nf_req, nf_res);
    }
};

class http_test : public ::testing::Test {
protected:
    id_seed seed;
    const std::string res_body = "http_response body";
    const std::string cyrillic_res_body = "Поиск по сайту";
    const std::chrono::hours cache_ttl = std::chrono::hours(24);
    const std::chrono::hours outdated_cache_ttl = std::chrono::hours(-24);
    const http_request req{create_id(seed), "url", "POST", "http_request body", {{"header 1", "value 1"}}};
    const task response_task{process_http_response};

    SQLite::Database database = SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
    queue<task> task_queue;
    std::shared_ptr<::testing::NiceMock<nf_client_mock>> nf_client = std::make_shared<::testing::NiceMock<nf_client_mock>>();
    http_client client = {nf_client};

    virtual void SetUp() {
        init_http_client_cache(database);
    }
};

TEST_F(http_test, should_send_specified_request) {
    http_response res{req, res_body, nativeformat::http::StatusCodeOK};
    nf_client->mock_response(req, res.body);
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
    EXPECT_EQ(res, *client.response_queue.try_dequeue());
    EXPECT_EQ(response_task, *task_queue.try_dequeue());
}

TEST_F(http_test, should_clear_requests_to_send_upon_sending_them) {
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
    EXPECT_TRUE(client.requests_to_send.empty());
}

TEST_F(http_test, should_send_multiple_requests_and_get_responses_from_network_and_then_from_cache) {
    std::vector<http_response> ress;
    std::vector<http_request> reqs;
    for (auto i = 0; i < 2; ++i) {
        const auto num = std::to_string(i + 1);
        http_request req{create_id(seed), "url " + num};
        req.cache_ttl = cache_ttl;
        ress.push_back({req, "http_response " + num, nativeformat::http::StatusCodeOK});
        nf_client->mock_response(req, ress[i].body);
        reqs.push_back(req);
    }
    for (auto i = 0; i < 2; i++) {
        client.requests_to_send = reqs;
        send_http_requests(client, database, task_queue);
        for (auto j = 0; j < reqs.size(); j++) {
            EXPECT_EQ(ress[j], *client.response_queue.try_dequeue());
        }
    }
}

TEST_F(http_test, should_return_empty_string_as_response_body_if_response_length_is_zero) {
    http_response res{req, "", nativeformat::http::StatusCodeOK};
    const auto nf_req = nf_client->to_nf_request(req);
    const auto nf_res = std::make_shared<nativeformat::http::ResponseImplementation>(
            nf_req,
            nullptr,
            0,
            nativeformat::http::StatusCodeOK,
            false);
    nf_client->mock_response(nf_req, nf_res);
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
    EXPECT_EQ(res, *client.response_queue.try_dequeue());
}

TEST_F(http_test, should_store_response_in_cache_and_reuse_it_for_following_requests) {
    http_request req;
    req.cache_ttl = cache_ttl;
    http_response res{req, res_body, nativeformat::http::StatusCodeOK};
    nf_client->mock_response(req, res_body);
    EXPECT_CALL(*nf_client, performRequest(::testing::_, ::testing::_)).Times(1);
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
    client.response_queue.try_dequeue();
    task_queue.try_dequeue();
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
    EXPECT_EQ(res, *client.response_queue.try_dequeue());
    EXPECT_EQ(response_task, *task_queue.try_dequeue());
}

TEST_F(http_test, should_cache_response_for_duration_specified_in_request) {
    http_request req;
    req.cache_ttl = outdated_cache_ttl;
    nf_client->mock_response(req, res_body);
    EXPECT_CALL(*nf_client, performRequest(::testing::_, ::testing::_)).Times(2);
    client.requests_to_send.insert(client.requests_to_send.end(), {req, req});
    send_http_requests(client, database, task_queue);
}

TEST_F(http_test, should_not_use_same_cached_response_for_different_requests) {
    std::vector<http_request> reqs = {
            {create_id(seed), req.url},
            {create_id(seed), req.url, req.method},
            {create_id(seed), req.url, req.method, req.body},
            {create_id(seed), req.url, req.method, req.body, {{"header 1", "value 1"}, {"header 2", "value 2"}}}
    };
    EXPECT_CALL(*nf_client, performRequest(::testing::_, ::testing::_)).Times(reqs.size());
    for (auto& req: reqs) {
        req.cache_ttl = cache_ttl;
        nf_client->mock_response(req, res_body);
        client.requests_to_send.push_back(req);
    }
    send_http_requests(client, database, task_queue);
}

TEST_F(http_test, should_miss_cache_fail_network_request_and_get_outdated_response_from_cache) {
    http_request req;
    req.cache_ttl = outdated_cache_ttl;
    http_response res{req, res_body, nativeformat::http::StatusCodeOK};
    nf_client->mock_response(req, res_body);
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
    client.response_queue = queue<http_response>();
    for (auto code: {nativeformat::http::StatusCodeBadRequest, nativeformat::http::StatusCodeInternalServerError}) {
        nf_client->mock_response(req, "", code);
        client.requests_to_send.push_back(req);
        send_http_requests(client, database, task_queue);
        EXPECT_EQ(res, *client.response_queue.try_dequeue());
    }
}

TEST_F(http_test, should_miss_cache_and_fail_network_request) {
    http_response res{req, res_body, nativeformat::http::StatusCodeBadRequest};
    nf_client->mock_response(req, res_body, nativeformat::http::StatusCodeBadRequest);
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
    EXPECT_EQ(res, *client.response_queue.try_dequeue());
}

TEST_F(http_test, two_requests_should_not_be_equal) {
    http_request req1;
    http_request req2{create_id(seed), req.url};
    http_request req3{create_id(seed), req.url, req.method};
    http_request req4{create_id(seed), req.url, req.method, req.body};
    http_request req5{create_id(seed), req.url, req.method, req.body, req.headers, cache_ttl};
    EXPECT_NE(req, req1);
    EXPECT_NE(req, req2);
    EXPECT_NE(req, req3);
    EXPECT_NE(req, req4);
    EXPECT_NE(req, req5);
}


TEST_F(http_test, two_equal_requests_with_different_ids_should_be_equal) {
    auto req1 = req;
    req1.id = create_id(seed);
    auto req2 = req;
    req2.id = create_id(seed);
    EXPECT_EQ(req1, req2);
}

TEST_F(http_test, two_responses_should_not_be_equal) {
    http_response res{req, res_body, nativeformat::http::StatusCodeBadRequest};
    http_response res1{req};
    http_response res2{req, res_body};
    EXPECT_NE(res, res1);
    EXPECT_NE(res, res2);
}

TEST_F(http_test, should_assign_random_user_agent_from_the_specified_list) {
    client.user_agents = {"UA1", "UA2"};
    for (const auto& ua: client.user_agents) {
        const auto nf_req = nf_client->to_nf_request(req, ua);
        EXPECT_CALL(*nf_client, performRequest(is_request(nf_req), ::testing::_)).Times(::testing::AtLeast(1));
    }
    client.requests_to_send = std::vector<http_request>(client.user_agents.size() * 3, req);
    send_http_requests(client, database, task_queue);
}

TEST_F(http_test, should_not_override_specified_custom_user_agent) {
    http_request req;
    req.headers = {{USER_AGENT_HTTP_HEADER, "custom UA"}};
    client.user_agents = {"UA1", "UA2"};
    const auto nf_req = nf_client->to_nf_request(req);
    EXPECT_CALL(*nf_client, performRequest(is_request(nf_req), ::testing::_));
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
}

TEST_F(http_test, should_convert_cp_1251_response_into_utf) {
    const auto win_res = iconvlite::utf2cp(cyrillic_res_body);
    const http_response res{req, cyrillic_res_body, nativeformat::http::StatusCodeOK};
    const std::vector<std::string> content_types = {"something-something/windows-1251", "WINDOWS-1251"};
    for (const auto& content_type: content_types) {
        nf_client->mock_response(req, win_res, nativeformat::http::StatusCodeOK, content_type);
        client.requests_to_send.push_back(req);
        send_http_requests(client, database, task_queue);
        EXPECT_EQ(res, *client.response_queue.try_dequeue());
    }
}

TEST_F(http_test, should_not_attempt_to_convert_response_if_content_type_is_unknown) {
    http_response res{req, iconvlite::utf2cp(cyrillic_res_body), nativeformat::http::StatusCodeOK};
    nf_client->mock_response(req, res.body);
    client.requests_to_send.push_back(req);
    send_http_requests(client, database, task_queue);
    EXPECT_EQ(res, *client.response_queue.try_dequeue());
}
