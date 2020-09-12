add_library(Core STATIC
        include/tv-show-storage/TvShowStorage.h
        include/tv-show-storage/TvShow.h
        include/tv-show-storage/LastWatchDate.h
        include/tv-show-storage/ListPage.h
        include/tv-show-storage/BigListProxy.h
        include/tv-show-storage/Episode.h
        src/tv-show-storage/Episode.cpp
        src/tv-show-storage/TvShow.cpp
        src/tv-show-storage/LastWatchDate.cpp
        include/http-client/HttpClient.h
        include/tv-show-crawler-storage/TvShowCrawlerStorage.h
        include/tv-show-crawler-runtime/TvShowCrawlerRuntime.h
        include/tv-show-crawler-runtime/CrawlerSerializer.h
        include/tv-show-crawler-runtime/model/CrawlerStep.h
        include/tv-show-crawler-runtime/model/Crawler.h
        include/tv-show-crawler-runtime/model/TvShowCrawler.h
        include/tv-show-crawler-runtime/model/CrawlerStepType.h
        include/tv-show-crawler-runtime/executor/RegExpStepExecutor.h
        include/tv-show-crawler-runtime/executor/CrawlerExecutor.h
        include/tv-show-crawler-runtime/executor/CrawlerStepExecutor.h
        include/tv-show-crawler-runtime/executor/ValueStepExecutor.h
        include/tv-show-crawler-runtime/executor/TransformStepExecutor.h
        include/tv-show-crawler-runtime/executor/FetchStepExecutor.h
        src/tv-show-crawler-runtime/TvShowCrawlerRuntime.cpp
        src/tv-show-crawler-runtime/CrawlerSerializer.cpp
        src/tv-show-crawler-runtime/model/CrawlerStep.cpp
        src/tv-show-crawler-runtime/model/Crawler.cpp
        src/tv-show-crawler-runtime/model/TvShowCrawler.cpp
        src/tv-show-crawler-runtime/model/CrawlerStepType.cpp
        src/tv-show-crawler-runtime/executor/CrawlerExecutor.cpp
        src/tv-show-crawler-runtime/executor/ValueStepExecutor.cpp
        src/tv-show-crawler-runtime/executor/TransformStepExecutor.cpp
        src/tv-show-crawler-runtime/executor/FetchStepExecutor.cpp
        src/tv-show-crawler-runtime/executor/RegExpStepExecutor.cpp
        include/tv-show-crawler-editor/TvShowCrawlerEditor.h
        src/tv-show-crawler-editor/TvShowCrawlerEditor.cpp
        )
target_include_directories(Core PUBLIC include PUBLIC lib/json)

add_executable(CoreTest
        test/mocks/TvShowCrawlerStorageMock.h
        test/mocks/HttpClientMock.h
        test/mocks/TvShowStorageMock.h
        test/mocks/TvShowCrawlerRuntimeMock.h
        test/tv-show-storage/TvShowTest.cpp
        test/tv-show-storage/LastWatchDateTest.cpp
        test/tv-show-storage/ListPageTest.cpp
        test/tv-show-storage/BigListProxyTest.cpp
        test/tv-show-storage/EpisodeTest.cpp
        test/tv-show-crawler-runtime/model/CrawlerStepTest.cpp
        test/tv-show-crawler-runtime/model/CrawlerTest.cpp
        test/tv-show-crawler-runtime/model/TvShowCrawlerTest.cpp
        test/tv-show-crawler-runtime/TvShowCrawlerRuntimeTest.cpp
        test/tv-show-crawler-runtime/model/CrawlerStepTypeTest.cpp
        test/tv-show-crawler-editor/TvShowCrawlerEditorTest.cpp)
target_include_directories(CoreTest PUBLIC test/mocks)
target_link_libraries(CoreTest Core gtest gmock gtest_main)