#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <crawler-runtime/CrawlerRuntime.h>
#include <CrawlerHttpClientMock.h>

class CrawlerRuntimeTest : public ::testing::Test {
protected:
    const std::chrono::milliseconds networkCacheTtl = std::chrono::milliseconds(1);
    mocks::CrawlerHttpClientMock httpClient;
    serio::CrawlerRuntime runtime = serio::CrawlerRuntime(httpClient);
};

TEST_F(CrawlerRuntimeTest, shouldExecuteTwoReturnOnlyCrawlers) {
    const std::vector<serio::Crawler> crawlers = {
        serio::Crawler{"function crawl() {return [1, 'text', false];}", networkCacheTtl},
        serio::Crawler{"function crawl() {return {a: 1, b: 'text', c: false};}", networkCacheTtl}
    };
    const std::vector<nlohmann::json> expected = {
        {1, "text", false},
        {{"a", 1}, {"b", "text"}, {"c", false}}
    };
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, oneCrawlerShouldFinishBeforeTheOther) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {let a = 5 + 1; let b = a + 5; return [1, 'text', false];}", networkCacheTtl},
            serio::Crawler{"function crawl() {return {a: 1, b: 'text', c: false};}", networkCacheTtl}
    };
    const std::vector<nlohmann::json> expected = {
            {1, "text", false},
            {{"a", 1}, {"b", "text"}, {"c", false}}
    };
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldIgnoreCrawlerWithCodeSyntaxErrors) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function ex{(123-=", networkCacheTtl},
            serio::Crawler{"function crawl() {return {a: 1, b: 'text', c: false};}", networkCacheTtl}
    };
    const std::vector<nlohmann::json> expected = {{{"a", 1}, {"b", "text"}, {"c", false}}};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldIgnoreCrawlersExecutionOfWhichHasFailed) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function notCrawl() {return [];}", networkCacheTtl},
            serio::Crawler{"function crawl() {return {a: 1, b: 'text', c: false};}", networkCacheTtl}
    };
    const std::vector<nlohmann::json> expected = {{{"a", 1}, {"b", "text"}, {"c", false}}};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldIgnoreCrawlerExecutionResultsThatCantBeParsedAsJson) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {return function() {return 1;};}", networkCacheTtl},
            serio::Crawler{"function crawl() {return {a: 1, b: 'text', c: false};}", networkCacheTtl}
    };
    const std::vector<nlohmann::json> expected = {{{"a", 1}, {"b", "text"}, {"c", false}}};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}