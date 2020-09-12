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

class TvShowCrawlerRuntimeTest : public ::testing::Test {
protected:
    const std::string friends = "Friends";
    const std::string mandalorian = "Mandalorian";
    const std::string thumbnailUrl = "https://tv-show/thumbnail.jpg";
    std::promise<std::vector<std::string>> httpClientResponsePromise;
    ::testing::NiceMock<HttpClientMock> httpClient;
    ::testing::NiceMock<TvShowCrawlerStorageMock> crawlerStorage;
    ::testing::NiceMock<TvShowStorageMock> tvShowStorage;
    serio::core::TvShowCrawlerRuntime runtime = serio::core::TvShowCrawlerRuntime(crawlerStorage, tvShowStorage, httpClient);
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
        std::string httpClientResponse = "Tv show image='thumbnail.jpg' is nice. Episodes are: episode-1.mp4, episode-2.mp4 and episode-3.mp4";
        std::promise<std::vector<std::string>> promise;
        promise.set_value({httpClientResponse});
        ON_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"url"})))
            .WillByDefault(::testing::Return(::testing::ByMove(promise.get_future())));
        httpClientResponsePromise.set_value({httpClientResponse});
    }
};

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithoutStepsInStorage) {
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(friends, R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[]},"showName":"Friends","thumbnailCrawler":{"steps":[]}})"));
    serio::core::TvShowCrawler crawler(friends, emptyCrawler);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithStepsForEpisodeVideoInStorage) {
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(mandalorian, R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[{"type":"value","value":"url"},{"type":"fetch"}]},"showName":"Mandalorian","thumbnailCrawler":{"steps":[]}})"));
    serio::core::TvShowCrawler crawler(mandalorian, crawlerWithSteps);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithStepsForThumbnailInStorage) {
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(mandalorian, R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[]},"showName":"Mandalorian","thumbnailCrawler":{"steps":[{"type":"value","value":"url"},{"type":"fetch"}]}})"));
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
    try {
        runtime.crawlTvShowAndSaveCrawler(crawler);
        FAIL();
    } catch (serio::core::TvShowCrawlerExecutionError& e) {
        EXPECT_STREQ("Failed to crawl 'Mandalorian': Failed to execute episode video crawler: "
                     "Failed to execute step #2: Timeout", e.what());
    }
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
    try {
        runtime.crawlTvShowAndSaveCrawler(crawler);
        FAIL();
    } catch (serio::core::TvShowCrawlerExecutionError& e) {
        EXPECT_STREQ("Failed to crawl 'Friends': Can't assign crawled episode names to episode video URLs: "
                     "video URLs count = 3 episode names count = 2", e.what());
    }
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
