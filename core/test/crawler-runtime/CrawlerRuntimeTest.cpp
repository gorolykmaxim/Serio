#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <crawler-runtime/CrawlerRuntime.h>
#include <CrawlerHttpClientMock.h>

class CrawlerRuntimeTest : public ::testing::Test {
protected:
    const std::chrono::milliseconds networkCacheTtl = std::chrono::milliseconds(1);
    const nlohmann::json expectedResult = {{"a", 1}, {"b", "text"}, {"c", false}};
    const serio::Crawler workingCrawler{"function crawl() {return {a: 1, b: 'text', c: false};}", networkCacheTtl};
    ::testing::NiceMock<mocks::CrawlerHttpClientMock> httpClient;
    serio::CrawlerRuntime runtime = serio::CrawlerRuntime(httpClient, true);

    void mockHttpClientResponse(const serio::HttpRequest& request, const std::string& response) {
        serio::HttpResponse httpResponse(response);
        ON_CALL(httpClient, sendRequest(request, networkCacheTtl)).WillByDefault(::testing::Return(httpResponse));
    }

    void mockHttpClientResponse(const serio::HttpRequest& request, const std::runtime_error& exception) {
        serio::HttpResponse httpResponse([exception] () -> std::string { throw exception; });
        ON_CALL(httpClient, sendRequest(request, networkCacheTtl)).WillByDefault(::testing::Return(httpResponse));
    }
};

TEST_F(CrawlerRuntimeTest, shouldExecuteTwoReturnOnlyCrawlers) {
    const std::vector<serio::Crawler> crawlers = {
        serio::Crawler{"function crawl() {return [1, 'text', false];}", networkCacheTtl},
        serio::Crawler{"function crawl() {return {a: 1, b: 'text', c: false};}", networkCacheTtl}
    };
    const std::vector<nlohmann::json> expected = {{1, "text", false}, expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, oneCrawlerShouldFinishBeforeTheOther) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {let a = 5 + 1; let b = a + 5; return [1, 'text', false];}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {{1, "text", false}, expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldIgnoreCrawlerWithCodeSyntaxErrors) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function ex{(123-=", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldIgnoreCrawlersExecutionOfWhichHasFailed) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function notCrawl() {return [];}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldIgnoreCrawlerExecutionResultsThatCantBeParsedAsJson) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {return function() {return 1;};}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldBeAbleToAcceptArguments) {
    nlohmann::json arguments = {1, "text", false};
    serio::Crawler crawler{"function crawl(arguments) {return [arguments[0], arguments[1], arguments[2]];}", networkCacheTtl, arguments};
    const std::vector<nlohmann::json> expected = {arguments};
    const auto actual = runtime.executeCrawlers({crawler});
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldThrowAnErrorWhenAttemptingToPassCrawlerArgumentsOfUnsupportedType) {
    nlohmann::json arguments = {"something", {{"objectAttributeName", "objectAttributeValue"}}};
    serio::Crawler crawler{"function crawl() {return {};}", networkCacheTtl, arguments};
    EXPECT_THROW(runtime.executeCrawlers({crawler}), std::logic_error);
}

TEST_F(CrawlerRuntimeTest, shouldThrowAnErrorWhenAttemptingToPassJsonObjectInsteadOfArrayOfCrawlerArguments) {
    nlohmann::json arguments = std::map<std::string, std::string>({{"objectAttributeName", "objectAttributeValue"}});
    serio::Crawler crawler{"function crawl() {return {};}", networkCacheTtl, arguments};
    EXPECT_THROW(runtime.executeCrawlers({crawler}), std::logic_error);
}

TEST_F(CrawlerRuntimeTest, shouldBeAbleToExecuteRegularExpressionsWithoutGroups) {
    const std::vector<nlohmann::json> expected = {{"1", "2", "3"}};
    const serio::Crawler crawler{"function crawl() {return regExp('[0-9]+', ['kfjasndf1fjkasdf', 'nafjksdnfkj2sadf', 'fk3klas']);}"};
    const auto actual = runtime.executeCrawlers({crawler});
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldBeAbleToExecuteRegularExpressionsWithGroups) {
    const std::vector<nlohmann::json> expected = {{"1", "word", "2", "kekw", "3", "a"}};
    const serio::Crawler crawler{"function crawl() {return regExp('([0-9]+)-([a-z]+)', ['1-word2-kekw abasd3-a']);}"};
    const auto actual = runtime.executeCrawlers({crawler});
    EXPECT_EQ(expected, actual);
}


TEST_F(CrawlerRuntimeTest, shouldFailToExecuteRegExpWithRegExpNotBeingString) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {return regExp(null, ['asdf']);}", networkCacheTtl},
            serio::Crawler{"function crawl() {return regExp(true, ['false']);}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldFailToExecuteRegExpWithDataArrayNotBeingAnArrayOfStrings) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {return regExp('[a-z]+', 12442);}", networkCacheTtl},
            serio::Crawler{"function crawl() {return regExp('[a-z]+', ['123', 4124]);}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldExecuteHttpRequestWithoutOptionalArguments) {
    const std::string url = "https://hello-world.com";
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {return httpRequests([{url:'" + url + "'}]);}", networkCacheTtl},
            workingCrawler
    };
    const auto response = "Response data";
    const std::vector<nlohmann::json> expected = {{response}, expectedResult};
    mockHttpClientResponse(serio::HttpRequest{url}, response);
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldExecuteHttpRequestsOfTwoCrawlersConcurrentlyWhilePausingExecutionOfCrawlersThatAreWaitingForResponse) {
    const serio::HttpRequest req1{"https://hello-world.com"};
    const serio::HttpRequest req2{"https://dev.null"};
    const serio::HttpRequest req3{"https://test.org"};
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {let a = 1; let b = 2; let c = a + b; return httpRequests([{url:'" + req1.url + "'}]);}", networkCacheTtl},
            serio::Crawler{"function crawl() {httpRequests([{url:'" + req2.url + "'}]); return httpRequests([{url:'" + req3.url + "'}]);}", networkCacheTtl},
    };
    const auto response = "Response data";
    const std::vector<nlohmann::json> expected = {{response}, {response}};
    mockHttpClientResponse(req1, response);
    mockHttpClientResponse(req2, response);
    mockHttpClientResponse(req3, response);
    ::testing::InSequence seq;
    EXPECT_CALL(httpClient, sendRequest(req2, networkCacheTtl));
    EXPECT_CALL(httpClient, sendRequest(req1, networkCacheTtl));
    EXPECT_CALL(httpClient, sendRequest(req3, networkCacheTtl));
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldFailToExecuteCrawlerThatPassesANonArrayToHttpRequests) {
    const auto crawlers = {
            serio::Crawler{"function crawl() {return httpRequests();}", networkCacheTtl},
            serio::Crawler{"function crawl() {return httpRequests({url:'kek'});}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldFailToExecuteCrawlerThatTriesToSendHttpRequestWithNonStringUrl) {
    const auto crawlers = {
            serio::Crawler{"function crawl() {return httpRequests([{}]);}", networkCacheTtl},
            serio::Crawler{"function crawl() {return httpRequests([{url:15}]);}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldFailToExecuteCrawlerDueToAnHttpFailure) {
    const serio::HttpRequest request{"https://hello-world.com"};
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {return httpRequests([{url:'" + request.url + "'}]);}", networkCacheTtl},
            workingCrawler
    };
    mockHttpClientResponse(request, serio::HttpResponseError(request.url, 503, "timeout"));
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldSendRequestWithTheSpecifiedMethod) {
    const serio::HttpRequest request{"https://hello-world.com", "POST"};
    const serio::Crawler crawler{"function crawl() {return httpRequests([{url:'" + request.url + "', method:'" + request.method + "'}]);}", networkCacheTtl};
    const auto response = "Response data";
    const std::vector<nlohmann::json> expected = {{response}};
    mockHttpClientResponse(request, response);
    const auto actual = runtime.executeCrawlers({crawler});
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldSendRequestWithTheSpecifiedBody) {
    const serio::HttpRequest request{"https://hello-world.com", "", {}, "request body"};
    const serio::Crawler crawler{"function crawl() {return httpRequests([{url:'" + request.url + "', body:'" + request.body + "'}]);}", networkCacheTtl};
    const auto response = "Response data";
    const std::vector<nlohmann::json> expected = {{response}};
    mockHttpClientResponse(request, response);
    const auto actual = runtime.executeCrawlers({crawler});
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldSendRequestWithTheSpecifiedHeaders) {
    const serio::HttpRequest request{"https://hello-world.com", "", {{"header", "value"}}};
    const serio::Crawler crawler{"function crawl() {return httpRequests([{url:'" + request.url + "', headers:{header: 'value'}}]);}", networkCacheTtl};
    const auto response = "Response data";
    const std::vector<nlohmann::json> expected = {{response}};
    mockHttpClientResponse(request, response);
    const auto actual = runtime.executeCrawlers({crawler});
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldFailToExecuteCrawlerWithHttpRequestWithANonStringMethod) {
    const auto crawlers = {
            serio::Crawler{"function crawl() {return httpRequests([{url:'url', method:15}]);}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldFailToExecuteCrawlerWithHttpRequestWithANonStringBody) {
    const auto crawlers = {
            serio::Crawler{"function crawl() {return httpRequests([{url:'url', body:15}]);}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}