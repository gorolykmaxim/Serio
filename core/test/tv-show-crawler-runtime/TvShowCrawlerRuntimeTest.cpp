#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>
#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include <tv-show-crawler-runtime/model/CrawlerStepType.h>
#include <tv-show-storage/TvShowStorage.h>
#include <http-client/HttpClient.h>
#include <HttpClientMock.h>
#include <TvShowCrawlerStorageMock.h>
#include <TvShowStorageMock.h>
#include <tv-show-crawler-runtime/model/CrawlLogEntry.h>

class TvShowCrawlerRuntimeTest : public ::testing::Test {
protected:
    const std::string friends = "Friends";
    const std::string mandalorian = "Mandalorian";
    const std::string thumbnailUrl = "https://tv-show/thumbnail.jpg";
    const std::string emptyFriendsCrawler = R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[]},"showName":"Friends","thumbnailCrawler":{"steps":[]}})";
    const std::string mandalorianCrawlerWithEpisodeVideoCrawler = R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[{"type":"value","value":"url"},{"type":"fetch"}]},"showName":"Mandalorian","thumbnailCrawler":{"steps":[]}})";
    const std::string mandalorianCrawlerWithThumbnailCrawler = R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[]},"showName":"Mandalorian","thumbnailCrawler":{"steps":[{"type":"value","value":"url"},{"type":"fetch"}]}})";
    const std::string httpClientResponse = "Tv show image='thumbnail.jpg' is nice. Episodes are: episode-1.mp4, episode-2.mp4 and episode-3.mp4";
    std::promise<std::vector<std::string>> httpClientResponsePromise;
    ::testing::NiceMock<HttpClientMock> httpClient;
    ::testing::NiceMock<TvShowCrawlerStorageMock> crawlerStorage;
    ::testing::NiceMock<TvShowStorageMock> tvShowStorage;
    serio::core::TvShowCrawlerRuntime runtime = serio::core::TvShowCrawlerRuntime(crawlerStorage, tvShowStorage, httpClient, 50);
    serio::core::Crawler emptyCrawler;
    serio::core::Crawler crawlerWithSteps = serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "url"}}),
        serio::core::CrawlerStep("fetch")
    });
    serio::core::Crawler episodeVideoCrawler = serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("fetch"),
        serio::core::CrawlerStep("regExp", {{"regExp", "[a-z\\-0-9]+\\.mp4"}}),
        serio::core::CrawlerStep("transform", {{"template", "https://tv-show/%s"}})
    });
    virtual void SetUp() {
        std::promise<std::vector<std::string>> promise;
        promise.set_value({httpClientResponse});
        ON_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"url"})))
            .WillByDefault(::testing::Return(::testing::ByMove(promise.get_future())));
        httpClientResponsePromise.set_value({httpClientResponse});
    }
    void expectTvShowCrawlToFail(const serio::core::TvShowCrawler& crawler, const char* errorMessage) {
        try {
            runtime.crawlTvShowAndSaveCrawler(crawler);
            FAIL();
        } catch (std::runtime_error& e) {
            EXPECT_STREQ(errorMessage, e.what());
        }
    }
    void expectCrawlerDeserializationToFail(const std::string& rawTvShowCrawler, const char* errorMessage) {
        try {
            (void)runtime.deserializeTvShowCrawler(rawTvShowCrawler);
            FAIL();
        } catch (std::runtime_error& e) {
            EXPECT_STREQ(errorMessage, e.what());
        }
    }
};

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithoutStepsInStorage) {
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(friends, emptyFriendsCrawler));
    serio::core::TvShowCrawler crawler(friends, emptyCrawler);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithStepsForEpisodeVideoInStorage) {
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(mandalorian, mandalorianCrawlerWithEpisodeVideoCrawler));
    serio::core::TvShowCrawler crawler(mandalorian, crawlerWithSteps);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithStepsForThumbnailInStorage) {
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(mandalorian, mandalorianCrawlerWithThumbnailCrawler));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, crawlerWithSteps);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldReturnAllExistingCrawlerTypes) {
    std::vector<serio::core::CrawlerStepType> expectedTypes = {
            serio::core::CrawlerStepType("fetch", R"(Step with type "fetch" treats every string from the input array as a link, fetches content of each link and returns an array of fetched contents.)"),
            serio::core::CrawlerStepType("value", R"(Step with type "value" ignores the input data and returns an array with a single string, specified in the "value" property.)", {"value"}),
            serio::core::CrawlerStepType("transform", R"(Step with type "transform" will transform each string from the input array using the "template" property and return the results. Let's say that you have a following array of input strings: ['1', '2', '3']. A transform step with "template"="Episode %s" will turn that array into ['Episode 1', 'Episode 2', 'Episode 3'].)", {"template"}),
            serio::core::CrawlerStepType("regExp", "Step with type \"regExp\" applies the regular expression, specified in \"regExp\" property, to every string from the input array, and returns an array of all found strings across all the input strings. Regular expressions can return groups of results, if you specify \"(.*)\" as a part of an expression. Normally this step returns all matches from the group 0 but if the aforementioned construction is present in the expression - the group 0 will be ignored and only the other groups (that correspond to such expressions) will be returned.", {"regExp"})
    };
    EXPECT_THAT(runtime.getCrawlerStepTypes(), ::testing::UnorderedElementsAreArray(expectedTypes));
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithoutEpisodesAndThumbnail) {
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(friends), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(friends, emptyCrawler);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}


TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlThumbnailWithCrawlerStepOfUnknownType) {
    serio::core::TvShowCrawler crawler(friends, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("evaporate")
    }));
    EXPECT_THROW(runtime.crawlTvShowAndSaveCrawler(crawler), std::logic_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlThumbnailWithValueCrawlerStepNotHavingValueProperty) {
    serio::core::TvShowCrawler crawler(friends, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value")
    }));
    EXPECT_THROW(runtime.crawlTvShowAndSaveCrawler(crawler), std::logic_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithThumbnailHardcoded) {
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, thumbnailUrl), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", thumbnailUrl}})
    }));
    runtime.crawlTvShowAndSaveCrawler(crawler);
}


TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlThumbnailWithTransformStepNotHavingTemplateProperty) {
    serio::core::TvShowCrawler crawler(friends, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("transform")
    }));
    EXPECT_THROW(runtime.crawlTvShowAndSaveCrawler(crawler), std::logic_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlThumbnailWithRegExpStepNotHavingRegExpProperty) {
    serio::core::TvShowCrawler crawler(friends, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("regExp")
    }));
    EXPECT_THROW(runtime.crawlTvShowAndSaveCrawler(crawler), std::logic_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithThumbnailObtainedDynamically) {
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, thumbnailUrl), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("transform", {{"template", "%s/thumbnail.jpg"}})
    }));
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithThumbnailObtainedFromWebSiteUsingGroupMatch) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
        .WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, thumbnailUrl), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("fetch"),
        serio::core::CrawlerStep("regExp", {{"regExp", "image='(.*)'"}}),
        serio::core::CrawlerStep("transform", {{"template", "https://tv-show/%s"}})
    }));
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithThumbnailObtainedFromWebSiteUsingCompleteMatch) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
        .WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, thumbnailUrl), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("fetch"),
        serio::core::CrawlerStep("regExp", {{"regExp", "[a-z]+\\.jpg"}}),
        serio::core::CrawlerStep("transform", {{"template", "https://tv-show/%s"}})
    }));
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithoutThumbnailSinceTheCrawlersRegExpDidNotMatch) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
        .WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, ""), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("fetch"),
        serio::core::CrawlerStep("regExp", {{"regExp", "[0-9]+\\.jpg"}})
    }));
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithEpisodesObtainedFromWebSite) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
        .WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, ""), std::vector<serio::core::Episode>({
        serio::core::Episode(1, "https://tv-show/episode-1.mp4"),
        serio::core::Episode(2, "https://tv-show/episode-2.mp4"),
        serio::core::Episode(3, "https://tv-show/episode-3.mp4")
    })));
    serio::core::TvShowCrawler crawler(mandalorian, episodeVideoCrawler);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlTvShowDueToHttpClientError) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
        .WillOnce(::testing::Throw(std::runtime_error("Timeout")));
    EXPECT_CALL(tvShowStorage, saveTvShow(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(::testing::_, ::testing::_)).Times(0);
    serio::core::TvShowCrawler crawler(mandalorian, episodeVideoCrawler);
    expectTvShowCrawlToFail(crawler, "Failed to crawl 'Mandalorian': Failed to execute episode video crawler: "
                                     "Failed to execute step #2: Timeout");
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithEpisodesHavingCustomEpisodeNames) {
    httpClientResponsePromise = std::promise<std::vector<std::string>>();
    httpClientResponsePromise.set_value({"Tv show image='thumbnail.jpg' is nice. Episodes are: DarkDawn.mp4, StrangeDay.mp4 and SleeplessNight.mp4"});
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
        .WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, ""), std::vector<serio::core::Episode>({
        serio::core::Episode(1, "https://tv-show/DarkDawn.mp4", "DarkDawn"),
        serio::core::Episode(2, "https://tv-show/StrangeDay.mp4", "StrangeDay"),
        serio::core::Episode(3, "https://tv-show/SleeplessNight.mp4", "SleeplessNight")
    })));
    serio::core::Crawler episodeVideoCrawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("fetch"),
        serio::core::CrawlerStep("regExp", {{"regExp", "[a-zA-Z]+\\.mp4"}}),
        serio::core::CrawlerStep("transform", {{"template", "https://tv-show/%s"}})
    });
    serio::core::Crawler episodeNameCrawler({
        serio::core::CrawlerStep("regExp", {{"regExp", "([a-zA-Z]+)\\.mp4"}})
    });
    serio::core::TvShowCrawler crawler(mandalorian, episodeVideoCrawler, emptyCrawler, episodeNameCrawler);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToSaveTvShowWithEpisodesVideoUrlsCountNotEqualEpisodeNamesCount) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
        .WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(tvShowStorage, saveTvShow(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(::testing::_, ::testing::_)).Times(0);
    serio::core::Crawler episodeNameCrawler({
        serio::core::CrawlerStep("regExp", {{"regExp", "[1-2]"}}),
        serio::core::CrawlerStep("transform", {{"template", "The Episode #%s"}})
    });
    serio::core::TvShowCrawler crawler(friends, episodeVideoCrawler, emptyCrawler, episodeNameCrawler);
    expectTvShowCrawlToFail(crawler, "Failed to crawl 'Friends': Can't assign crawled episode names to episode video URLs: "
                                     "video URLs count = 3 episode names count = 2");
}

TEST_F(TvShowCrawlerRuntimeTest, shouldTellThatSpecifiedCrawlerWillNotOverrideAnyOfExistingTvShows) {
    serio::core::TvShowCrawler crawler(friends, emptyCrawler);
    EXPECT_FALSE(runtime.willOverrideExistingTvShow(crawler));
}

TEST_F(TvShowCrawlerRuntimeTest, shouldTellThatSpecifiedCrawlerWillOverrideExistingTvShow) {
    serio::core::TvShow tvShow(friends);
    EXPECT_CALL(tvShowStorage, getTvShowByName(friends)).WillOnce(::testing::Return(tvShow));
    serio::core::TvShowCrawler crawler(friends, emptyCrawler);
    EXPECT_TRUE(runtime.willOverrideExistingTvShow(crawler));
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToDeserializeCrawlerInInvalidFormat) {
    EXPECT_THROW((void)runtime.deserializeTvShowCrawler("{something}"), std::runtime_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToDeserializeTvShowCrawlerWithoutName) {
    expectCrawlerDeserializationToFail("{}", "Failed to deserialize TV show crawler: TV show name not specified");
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToDeserializeTvShowCrawlerWithEmptyName) {
    expectCrawlerDeserializationToFail(R"({"showName": ""})", "Failed to deserialize TV show crawler: TV show name is empty");
}

TEST_F(TvShowCrawlerRuntimeTest, shouldDeserializeEmptyTvShowCrawler) {
    serio::core::TvShowCrawler expectedCrawler(friends, emptyCrawler);
    EXPECT_EQ(expectedCrawler, runtime.deserializeTvShowCrawler(emptyFriendsCrawler));
    expectedCrawler = serio::core::TvShowCrawler(mandalorian, emptyCrawler);
    EXPECT_EQ(expectedCrawler, runtime.deserializeTvShowCrawler(R"({"showName": "Mandalorian"})"));
}

TEST_F(TvShowCrawlerRuntimeTest, shouldDeserializeTvShowCrawlerWithEpisodeVideoCrawlerSpecified) {
    serio::core::TvShowCrawler expectedCrawler(mandalorian, crawlerWithSteps);
    EXPECT_EQ(expectedCrawler, runtime.deserializeTvShowCrawler(mandalorianCrawlerWithEpisodeVideoCrawler));
}

TEST_F(TvShowCrawlerRuntimeTest, shouldDeserializeTvShowCrawlerWithThumbnailCrawlerSpecified) {
    serio::core::TvShowCrawler expectedCrawler(mandalorian, emptyCrawler, crawlerWithSteps);
    EXPECT_EQ(expectedCrawler, runtime.deserializeTvShowCrawler(mandalorianCrawlerWithThumbnailCrawler));
}

TEST_F(TvShowCrawlerRuntimeTest, shouldDeserializeTvShowCrawlerWithEpisodeNameCrawlerSpecified) {
    serio::core::TvShowCrawler expectedCrawler(mandalorian, emptyCrawler, emptyCrawler, crawlerWithSteps);
    EXPECT_EQ(expectedCrawler, runtime.deserializeTvShowCrawler(R"({"episodeNameCrawler":{"steps":[{"type":"value","value":"url"},{"type":"fetch"}]},"episodeVideoCrawler":{"steps":[]},"showName":"Mandalorian","thumbnailCrawler":{"steps":[]}})"));
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToDeserializeTvShowCrawlerWithStepWithoutTypeSpecified) {
    expectCrawlerDeserializationToFail(
            R"({"showName":"a","episodeVideoCrawler":{"steps":[{"a":"b"}]}})",
            R"(Failed to deserialize TV show crawler: Failed to deserialize "episodeVideoCrawler": Failed to deserialize crawler step #1: Crawler step type not specified)");
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToDeserializeTvShowCrawlerWithUnknownStepType) {
    expectCrawlerDeserializationToFail(
            R"({"showName":"a","thumbnailCrawler":{"steps":[{"type":"fetch"}, {"type":"unknown"}]}})",
            R"(Failed to deserialize TV show crawler: Failed to deserialize "thumbnailCrawler": Failed to deserialize crawler step #2: Unknown crawler step type "unknown")");
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToDeserializeTvShowCrawlerStepHavingMandatoryPropertyMissing) {
    expectCrawlerDeserializationToFail(
            R"({"showName":"a","episodeNameCrawler":{"steps":[{"type":"value"}]}})",
            R"(Failed to deserialize TV show crawler: Failed to deserialize "episodeNameCrawler": Failed to deserialize crawler step #1: Crawler step with type "value" missing following mandatory properties: "value")");
}

TEST_F(TvShowCrawlerRuntimeTest, shouldReturnResultsOfExecutionOfTheSpecifiedCrawler) {
    std::vector<std::string> expectedResults = {
            "https://tv-show/episode-1.mp4",
            "https://tv-show/episode-2.mp4",
            "https://tv-show/episode-3.mp4"
    };
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
            .WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_EQ(expectedResults, runtime.executeCrawlerForResult(episodeVideoCrawler).result);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToExecuteSpecifiedCrawlerDueToHttpClientError) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
            .WillOnce(::testing::Throw(std::runtime_error("Timeout")));
    try {
        (void)runtime.executeCrawlerForResult(episodeVideoCrawler);
        FAIL();
    } catch (std::runtime_error& e) {
        EXPECT_STREQ("Failed to execute specified crawler: Failed to execute step #2: Timeout", e.what());
    }
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToExecuteCrawlerWithCrawlerStepOfUnknownType) {
    serio::core::Crawler crawler({serio::core::CrawlerStep("evaporate")});
    EXPECT_THROW((void)runtime.executeCrawlerForResult(crawler), std::logic_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToExecuteCrawlerWithValueCrawlerStepNotHavingValueProperty) {
    serio::core::Crawler crawler({serio::core::CrawlerStep("value")});
    EXPECT_THROW((void)runtime.executeCrawlerForResult(crawler), std::logic_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToExecuteCrawlerWithTransformStepNotHavingTemplateProperty) {
    serio::core::Crawler crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("transform")
    });
    EXPECT_THROW((void)runtime.executeCrawlerForResult(crawler), std::logic_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToExecuteCrawlerWithRegExpStepNotHavingRegExpProperty) {
    serio::core::Crawler crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("regExp")
    });
    EXPECT_THROW((void)runtime.executeCrawlerForResult(crawler), std::logic_error);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldReturnExecutionLogOfTheSpecifiedCrawler) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"})))
        .WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    std::vector<serio::core::CrawlLogEntry> log = runtime.executeCrawlerForResult(episodeVideoCrawler).log;
    EXPECT_EQ("Executing value step with properties: 'value: https://tv-show'", log[0].getText());
    EXPECT_EQ("[]", log[0].getStepInputData());
    EXPECT_EQ("[https://tv-show]", log[0].getStepOutputData());
    EXPECT_EQ("Executing fetch step with properties: ''", log[1].getText());
    EXPECT_EQ("[https://tv-show]", log[1].getStepInputData());
    EXPECT_EQ("[Tv show image='thumbnail.jpg' is nice. Episode...", log[1].getStepOutputData());
    EXPECT_EQ("Executing regExp step with properties: 'regExp: [a-z\\-0-9]+\\.mp4'", log[2].getText());
    EXPECT_EQ("[Tv show image='thumbnail.jpg' is nice. Episode...", log[2].getStepInputData());
    EXPECT_EQ("[episode-1.mp4, episode-2.mp4, episode-3.mp4]", log[2].getStepOutputData());
    EXPECT_EQ("Executing transform step with properties: 'template: https://tv-show/%s'", log[3].getText());
    EXPECT_EQ("[episode-1.mp4, episode-2.mp4, episode-3.mp4]", log[3].getStepInputData());
    EXPECT_EQ("[https://tv-show/episode-1.mp4, https://tv-show...", log[3].getStepOutputData());
}
