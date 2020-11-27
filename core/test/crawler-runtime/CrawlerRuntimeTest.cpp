#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <crawler-runtime/CrawlerRuntime.h>
#include <CrawlerHttpClientMock.h>

class CrawlerRuntimeTest : public ::testing::Test {
protected:
    const std::chrono::milliseconds networkCacheTtl = std::chrono::milliseconds(1);
    const nlohmann::json expectedResult = {{"a", 1}, {"b", "text"}, {"c", false}};
    const serio::Crawler workingCrawler{"function crawl() {return {a: 1, b: 'text', c: false};}", networkCacheTtl};
    mocks::CrawlerHttpClientMock httpClient;
    serio::CrawlerRuntime runtime = serio::CrawlerRuntime(httpClient);
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


TEST_F(CrawlerRuntimeTest, shouldFailToExecuteRegExpWithFirstCallArgumentNotSpecified) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {return regExp(null, ['asdf']);}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}

TEST_F(CrawlerRuntimeTest, shouldFailToExecuteRegExpWithNonArraySecondCallArgument) {
    const std::vector<serio::Crawler> crawlers = {
            serio::Crawler{"function crawl() {return regExp('[a-z]+', 12442);}", networkCacheTtl},
            workingCrawler
    };
    const std::vector<nlohmann::json> expected = {expectedResult};
    const auto actual = runtime.executeCrawlers(crawlers);
    EXPECT_EQ(expected, actual);
}