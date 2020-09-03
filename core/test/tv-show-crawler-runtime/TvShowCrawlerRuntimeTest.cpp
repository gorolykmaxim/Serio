#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <tv-show-crawler-storage/TvShowCrawlerStorage.h>
#include <tv-show-crawler-runtime/TvShowCrawlerRuntime.h>
#include <tv-show-crawler-runtime/model/TvShowCrawler.h>
#include <tv-show-crawler-runtime/model/CrawlerStepType.h>
#include <tv-show-storage/TvShowStorage.h>
#include <http-client/HttpClient.h>

class HttpClientMock : public serio::core::HttpClient {
public:
    MOCK_METHOD((std::future<std::vector<std::string>>), fetchContentFromLinks, (const std::vector<std::string>&), (override));
};

class TvShowStorageMock : public serio::core::TvShowStorage {
public:
    MOCK_METHOD((serio::core::ListPage<serio::core::TvShow>), getAllTvShows, (unsigned int, unsigned int), (override));
    MOCK_METHOD((serio::core::ListPage<serio::core::TvShow>), getWatchedTvShows, (unsigned int, unsigned int), (override));
    MOCK_METHOD((serio::core::ListPage<serio::core::Episode>), getEpisodesOfTvShowWithName, (const std::string&, unsigned int, unsigned int), (override));
    MOCK_METHOD(void, saveTvShow, (const serio::core::TvShow&, const std::vector<serio::core::Episode>&), (override));
};

class TvShowCrawlerStorageMock : public serio::core::TvShowCrawlerStorage {
public:
    MOCK_METHOD((std::optional<std::string>), getTvShowCrawlerByTvShowName, (const std::string&), (override));
    MOCK_METHOD(void, saveTvShowCrawler, (const std::string&, const std::string&), (override));
};

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
        httpClientResponsePromise.set_value({"Tv show image='thumbnail.jpg' is nice. Episodes are: episode-1.mp4, episode-2.mp4 and episode-3.mp4"});
    }
};

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithoutStepsInStorage) {
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(friends, R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[]},"showName":"Friends","thumbnailCrawler":{"steps":[]}})"));
    serio::core::TvShowCrawler crawler(friends, emptyCrawler);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithStepsForEpisodeVideoInStorage) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"url"}))).WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(mandalorian, R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[{"type":"value","value":"url"},{"type":"fetch"}]},"showName":"Mandalorian","thumbnailCrawler":{"steps":[]}})"));
    serio::core::TvShowCrawler crawler(mandalorian, crawlerWithSteps);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveCrawlerWithStepsForThumbnailInStorage) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"url"}))).WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(mandalorian, R"({"episodeNameCrawler":{"steps":[]},"episodeVideoCrawler":{"steps":[]},"showName":"Mandalorian","thumbnailCrawler":{"steps":[{"type":"value","value":"url"},{"type":"fetch"}]}})"));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, crawlerWithSteps);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldReturnAllExistingCrawlerTypes) {
    std::vector<serio::core::CrawlerStepType> expectedTypes = {
            serio::core::CrawlerStepType("fetch"),
            serio::core::CrawlerStepType("value", {"value"}),
            serio::core::CrawlerStepType("transform", {"template"}),
            serio::core::CrawlerStepType("regExp", {"regExp"})
    };
    EXPECT_THAT(runtime.getCrawlerStepTypes(), ::testing::UnorderedElementsAreArray(expectedTypes));
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithoutEpisodesAndThumbnail) {
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(friends), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(friends, emptyCrawler);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}


TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlThumbnailWithCrawlerStepOfUnknownType) {
    try {
        serio::core::TvShowCrawler crawler(friends, emptyCrawler, serio::core::Crawler({
            serio::core::CrawlerStep("evaporate")
        }));
        runtime.crawlTvShowAndSaveCrawler(crawler);
        FAIL();
    } catch (serio::core::TvShowCrawlerExecutionError& e) {
        EXPECT_STREQ("Failed to crawl 'Friends': Failed to execute thumbnail crawler: Failed to execute step #1: Unknown crawler step type 'evaporate'", e.what());
    }
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlThumbnailWithValueCrawlerStepNotHavingValueProperty) {
    try {
        serio::core::TvShowCrawler crawler(friends, emptyCrawler, serio::core::Crawler({
            serio::core::CrawlerStep("value")
        }));
        runtime.crawlTvShowAndSaveCrawler(crawler);
        FAIL();
    } catch (serio::core::TvShowCrawlerExecutionError& e) {
        EXPECT_STREQ("Failed to crawl 'Friends': Failed to execute thumbnail crawler: Failed to execute step #1: Crawler step 'value' missing property 'value'", e.what());
    }
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithThumbnailHardcoded) {
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, thumbnailUrl), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", thumbnailUrl}})
    }));
    runtime.crawlTvShowAndSaveCrawler(crawler);
}


TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlThumbnailWithTransformStepNotHavingTemplateProperty) {
    try {
        serio::core::TvShowCrawler crawler(friends, emptyCrawler, serio::core::Crawler({
            serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
            serio::core::CrawlerStep("transform")
        }));
        runtime.crawlTvShowAndSaveCrawler(crawler);
        FAIL();
    } catch (serio::core::TvShowCrawlerExecutionError& e) {
        EXPECT_STREQ("Failed to crawl 'Friends': Failed to execute thumbnail crawler: Failed to execute step #2: Crawler step 'transform' missing property 'template'", e.what());
    }
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
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"}))).WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
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
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"}))).WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
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
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"}))).WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, ""), std::vector<serio::core::Episode>()));
    serio::core::TvShowCrawler crawler(mandalorian, emptyCrawler, serio::core::Crawler({
        serio::core::CrawlerStep("value", {{"value", "https://tv-show"}}),
        serio::core::CrawlerStep("fetch"),
        serio::core::CrawlerStep("regExp", {{"regExp", "[0-9]+\\.jpg"}})
    }));
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithEpisodesObtainedFromWebSite) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"}))).WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
    EXPECT_CALL(tvShowStorage, saveTvShow(serio::core::TvShow(mandalorian, ""), std::vector<serio::core::Episode>({
        serio::core::Episode(1, "https://tv-show/episode-1.mp4"),
        serio::core::Episode(2, "https://tv-show/episode-2.mp4"),
        serio::core::Episode(3, "https://tv-show/episode-3.mp4")
    })));
    serio::core::TvShowCrawler crawler(mandalorian, episodeVideoCrawler);
    runtime.crawlTvShowAndSaveCrawler(crawler);
}

TEST_F(TvShowCrawlerRuntimeTest, shouldFailToCrawlTvShowDueToHttpClientError) {
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"}))).WillOnce(::testing::Throw(std::runtime_error("Timeout")));
    EXPECT_CALL(tvShowStorage, saveTvShow(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(crawlerStorage, saveTvShowCrawler(::testing::_, ::testing::_)).Times(0);
    serio::core::TvShowCrawler crawler(mandalorian, episodeVideoCrawler);
    try {
        runtime.crawlTvShowAndSaveCrawler(crawler);
        FAIL();
    } catch (serio::core::TvShowCrawlerExecutionError& e) {
        EXPECT_STREQ("Failed to crawl 'Mandalorian': Failed to execute episode video crawler: Failed to execute step #2: Timeout", e.what());
    }
}

TEST_F(TvShowCrawlerRuntimeTest, shouldSaveTvShowWithEpisodesHavingCustomEpisodeNames) {
    httpClientResponsePromise = std::promise<std::vector<std::string>>();
    httpClientResponsePromise.set_value({"Tv show image='thumbnail.jpg' is nice. Episodes are: DarkDawn.mp4, StrangeDay.mp4 and SleeplessNight.mp4"});
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"}))).WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
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
    EXPECT_CALL(httpClient, fetchContentFromLinks(std::vector<std::string>({"https://tv-show"}))).WillOnce(::testing::Return(::testing::ByMove(httpClientResponsePromise.get_future())));
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
        EXPECT_STREQ("Failed to crawl 'Friends': Can't assign crawled episode names to episode video URLs: video URLs count = 3 episode names count = 2", e.what());
    }
}
