#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <crawler.h>
#include <http.h>
#include <id.h>

class crawler_test : public ::testing::Test {
protected:
    const std::string url = "https://url.com";
    const std::string method = "POST";
    const std::string body = "request body";
    const std::string res_body = "response body";
    const std::chrono::hours cache_ttl = std::chrono::hours(24);
    const std::string single_http_req_crawler_code = "function crawl() {return http([{url: '" + url + "'}]);}";
    id_seed seed;
    const std::vector<crawler_result> expected_results = {
            {create_id(seed), {{"crawler_result", 15}}},
            {create_id(seed), {{"data", true}}}
    };
    std::vector<http_response> incoming;
    std::vector<http_request> outgoing;
    crawler_runtime runtime;

    virtual void SetUp() {
        runtime.trace = true;
    }

    virtual void TearDown() {
        display_profiler_statistics(runtime.profiler_statistics);
    }
};

static void submit_crawlers_that_return(crawler_runtime& runtime, const std::vector<crawler_result>& results,
                                        id_seed& seed) {
    runtime.crawlers.reserve(results.size());
    for (const auto& expected_result: results) {
        runtime.crawlers.push_back({create_id(seed), "function crawl() {return " + expected_result.data.dump() + ";}"});
    }
}

static void expect_crawler_to_fail(const crawler& crawler, crawler_runtime& runtime) {
    id_seed seed;
    std::vector<http_response> incoming;
    std::vector<http_request> outgoing;
    runtime.crawlers.push_back(crawler);
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_TRUE(runtime.results.empty());
    EXPECT_TRUE(outgoing.empty());
}

static void expect_crawler_to_run_regex(const std::string& regex, const std::string& values,
                                        const std::vector<nlohmann::json>& expected, crawler_runtime& runtime) {
    id_seed seed;
    std::vector<http_response> incoming;
    std::vector<http_request> outgoing;
    runtime.crawlers.push_back({create_id(seed), "function crawl() {return regex('" + regex + "', " + values + ");}"});
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(expected, runtime.results[0].data);
}

static void expect_crawler_to_send_http_requests(const std::vector<http_request>& expected_requests,
                                                 const std::string& requests, crawler_runtime& runtime) {
    id_seed seed;
    std::vector<http_response> incoming;
    std::vector<http_request> outgoing;
    runtime.crawlers.push_back({create_id(seed), "function crawl() {return http(" + requests + ");}"});
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(expected_requests, outgoing);
}

TEST_F(crawler_test, should_return_value_specified_in_code) {
    submit_crawlers_that_return(runtime, expected_results, seed);
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(expected_results, runtime.results);
}


TEST_F(crawler_test, should_assign_crawler_id_to_crawler_result) {
    submit_crawlers_that_return(runtime, expected_results, seed);
    const auto crawlers = runtime.crawlers;
    execute_crawlers(runtime, incoming, outgoing, seed);
    for (auto i = 0; i < crawlers.size(); i++) {
        EXPECT_EQ(crawlers[i].id, runtime.results[i].id);
    }
}

TEST_F(crawler_test, should_clear_list_of_crawlers_after_accepting_them_for_execution) {
    submit_crawlers_that_return(runtime, expected_results, seed);
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_TRUE(runtime.crawlers.empty());
}

TEST_F(crawler_test, should_fail_to_parse_crawler_code) {
    expect_crawler_to_fail({create_id(seed), "function ex{(123-="}, runtime);
}

TEST_F(crawler_test, should_fail_to_execute_crawler) {
    expect_crawler_to_fail({create_id(seed), "function not_crawl() {return [];}"}, runtime);
}

TEST_F(crawler_test, should_fail_to_collect_results_of_crawler_execution) {
    expect_crawler_to_fail({create_id(seed), "function crawl() {return function () {return 1;};}"}, runtime);
}

TEST_F(crawler_test, should_pass_arguments_inside_crawler) {
    const std::vector<nlohmann::json> args = {1.5, true, "string"};
    runtime.crawlers.push_back({create_id(seed), "function crawl(args) {return args;}", args});
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(args, runtime.results[0].data);
}

TEST_F(crawler_test, should_fail_to_parse_argument_that_is_of_non_supported_type) {
    expect_crawler_to_fail({create_id(seed), "function crawl(args) {return args;}", {{{"key", "value"}}}}, runtime);
}

TEST_F(crawler_test, should_execute_regex) {
    expect_crawler_to_run_regex("[0-9]+",
                                "['yvu1bafj', 'yuv2yuuyv', '3uibu']",
                                {"1", "2", "3"},
                                runtime);
}

TEST_F(crawler_test, should_fail_to_execute_regex_if_expression_is_not_string) {
    expect_crawler_to_fail({create_id(seed), "function crawl(args) {return regex();}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl(args) {return regex(15);}"}, runtime);
}

TEST_F(crawler_test, should_fail_to_execute_regex_if_values_is_not_array_of_strings) {
    expect_crawler_to_fail({create_id(seed), "function crawl(args) {return regex('abc');}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl(args) {return regex('abc', 1245);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl(args) {return regex('abc', [true]);}"}, runtime);
}

TEST_F(crawler_test, should_fail_to_execute_regex_if_expression_is_invalid) {
    expect_crawler_to_fail({create_id(seed), "function crawl(args) {return regex('[', []);}"}, runtime);
}

TEST_F(crawler_test, should_execute_regex_with_groups) {
    expect_crawler_to_run_regex("([0-9]+)-([a-z]+)",
                                "['1-word2-kekw abasd3-a']",
                                {"1", "word", "2", "kekw", "3", "a"},
                                runtime);
}

TEST_F(crawler_test, should_send_http_requests) {
    expect_crawler_to_send_http_requests({{create_id(seed), url}, {create_id(seed), url + ".ua"}},
                                         "[{url: '" + url + "'}, {url: '" + url + ".ua'}]",runtime);
}

TEST_F(crawler_test, should_fail_to_send_http_requests_if_they_are_invalid) {
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: {}}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: '" + url + "'}, {}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: '" + url + "', method: {}}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: '" + url + "', body: {}}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: '" + url + "', headers: {}}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: '" + url + "', headers: [{}]}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: '" + url + "', headers: [{name: {}}]}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: '" + url + "', headers: [{name: 'header 1'}]}]);}"}, runtime);
    expect_crawler_to_fail({create_id(seed), "function crawl() {return http([{url: '" + url + "', headers: [{name: 'header 1', value: {}}]}]);}"}, runtime);
}

TEST_F(crawler_test, should_send_http_requests_with_crawlers_cache_ttl) {
    runtime.crawlers.push_back({create_id(seed), single_http_req_crawler_code, {}, cache_ttl});
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(cache_ttl, outgoing[0].cache_ttl);
}

TEST_F(crawler_test, should_send_http_request_with_specified_method) {
    expect_crawler_to_send_http_requests({{create_id(seed), url, method}},
                                         "[{url: '" + url + "', method: '" + method + "'}]", runtime);
}

TEST_F(crawler_test, should_send_http_request_with_specified_body) {
    expect_crawler_to_send_http_requests({{create_id(seed), url, method, body}},
                                         "[{url: '" + url + "', method: '" + method + "', body: '" + body + "'}]",
                                         runtime);
}

TEST_F(crawler_test, should_send_http_request_with_specified_headers) {
    expect_crawler_to_send_http_requests({{create_id(seed), url, method, body, {{"header 1", "value 1"}, {"header 2", "value 2"}}}},
                                         "[{url: '" + url + "', method: '" + method + "', body: '" + body + "', headers: [{name: 'header 1', value: 'value 1'}, {name: 'header 2', value: 'value 2'}]}]",
                                         runtime);
}

TEST_F(crawler_test, should_send_http_request_and_process_successful_http_response) {
    const nlohmann::json result = {res_body};
    runtime.crawlers.push_back({create_id(seed), single_http_req_crawler_code});
    execute_crawlers(runtime, incoming, outgoing, seed);
    incoming.push_back({outgoing[0], res_body, 200});
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(result, runtime.results[0].data);
}

TEST_F(crawler_test, should_send_multiple_http_requests_and_process_their_responses) {
    const nlohmann::json result = {res_body, res_body};
    runtime.crawlers.push_back({create_id(seed), "function crawl() {return http([{url: '" + url + "'}, {url: '" + url + "'}]);}"});
    execute_crawlers(runtime, incoming, outgoing, seed);
    incoming.push_back({outgoing[0], res_body, 200});
    execute_crawlers(runtime, incoming, outgoing, seed);
    incoming.push_back({outgoing[1], res_body, 200});
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(result, runtime.results[0].data);
}

TEST_F(crawler_test, should_ignore_http_responses_to_requests_that_were_not_made_by_runtime) {
    const nlohmann::json result = {res_body, res_body};
    runtime.crawlers.push_back({create_id(seed), single_http_req_crawler_code});
    execute_crawlers(runtime, incoming, outgoing, seed);
    incoming.push_back({outgoing[0], res_body, 200});
    execute_crawlers(runtime, incoming, outgoing, seed);
    incoming.push_back({{}, res_body, 200});
    execute_crawlers(runtime, incoming, outgoing, seed);
}

TEST_F(crawler_test, should_deliver_http_responses_to_appropriate_crawlers) {
    const std::vector<crawler_result> results = {{create_id(seed), {res_body}}, {create_id(seed), {res_body}}};
    runtime.crawlers.push_back({create_id(seed), single_http_req_crawler_code});
    runtime.crawlers.push_back({create_id(seed), single_http_req_crawler_code});
    execute_crawlers(runtime, incoming, outgoing, seed);
    for (auto i = 0; i < 2; i++) {
        incoming.push_back({outgoing[i], res_body, 200});
    }
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(results, runtime.results);
}

TEST_F(crawler_test, should_send_all_http_requests_of_crawler_with_the_same_cache_ttl) {
    runtime.crawlers.push_back({create_id(seed), "function crawl() {let a = http([{url: '" + url + "'}]); return http([{url: '" + url + "'}]);}", {}, cache_ttl});
    execute_crawlers(runtime, incoming, outgoing, seed);
    incoming.push_back({outgoing[0], res_body, 200});
    execute_crawlers(runtime, incoming, outgoing, seed);
    for (const auto& res: outgoing) {
        EXPECT_EQ(cache_ttl, res.cache_ttl);
    }
}

TEST_F(crawler_test, should_clear_http_configs_of_processed_crawlers) {
    const auto old_cache_ttl = std::chrono::hours(1);
    const auto id = create_id(seed);
    runtime.crawlers.push_back({id, single_http_req_crawler_code, {}, old_cache_ttl});
    execute_crawlers(runtime, incoming, outgoing, seed);
    incoming.push_back({outgoing[0], res_body, 200});
    execute_crawlers(runtime, incoming, outgoing, seed);
    runtime.crawlers.push_back({id, single_http_req_crawler_code, {}, cache_ttl});
    outgoing.clear();
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(cache_ttl, outgoing[0].cache_ttl);
}

TEST_F(crawler_test, should_clear_http_configs_of_failed_crawlers) {
    const auto old_cache_ttl = std::chrono::hours(1);
    const auto id = create_id(seed);
    runtime.crawlers.push_back({id, "function crawl() {", {}, old_cache_ttl});
    execute_crawlers(runtime, incoming, outgoing, seed);
    runtime.crawlers.push_back({id, single_http_req_crawler_code, {}, cache_ttl});
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_EQ(cache_ttl, outgoing[0].cache_ttl);
}

TEST_F(crawler_test, should_fail_to_execute_crawler_due_to_failed_http_response) {
    runtime.crawlers.push_back({create_id(seed), single_http_req_crawler_code});
    runtime.crawlers.push_back({create_id(seed), single_http_req_crawler_code});
    execute_crawlers(runtime, incoming, outgoing, seed);
    incoming.push_back({outgoing[0], res_body, 400});
    incoming.push_back({outgoing[1], res_body, 501});
    execute_crawlers(runtime, incoming, outgoing, seed);
    EXPECT_TRUE(runtime.results.empty());
    EXPECT_TRUE(runtime.pending_execs.empty());
}
