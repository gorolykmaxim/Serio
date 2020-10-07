add_library(Core STATIC
        include/tv-show-storage/TvShowStorage.h
        include/tv-show-storage/TvShow.h
        include/tv-show-storage/LastWatchDate.h
        include/tv-show-storage/ListPage.h
        include/tv-show-storage/BigListProxy.h
        include/tv-show-storage/Episode.h
        include/tv-show-storage/WatchProgress.h
        src/tv-show-storage/Episode.cpp
        src/tv-show-storage/TvShow.cpp
        src/tv-show-storage/LastWatchDate.cpp
        src/tv-show-storage/WatchProgress.cpp
        include/http-client/HttpClient.h
        include/tv-show-crawler-storage/TvShowCrawlerStorage.h
        include/tv-show-crawler-log-storage/TvShowCrawlerLogStorage.h
        include/tv-show-crawler-log-storage/CrawlLogEntry.h
        src/tv-show-crawler-log-storage/CrawlLogEntry.cpp
        include/tv-show-crawler-runtime/TvShowCrawlerRuntime.h
        include/tv-show-crawler-runtime/CrawlerSerializer.h
        include/tv-show-crawler-runtime/model/CrawlerStep.h
        include/tv-show-crawler-runtime/model/Crawler.h
        include/tv-show-crawler-runtime/model/TvShowCrawler.h
        include/tv-show-crawler-runtime/model/CrawlerStepType.h
        include/tv-show-crawler-runtime/model/CrawlResult.h
        include/tv-show-crawler-runtime/executor/RegExpStep.h
        include/tv-show-crawler-runtime/executor/CrawlerExecutor.h
        include/tv-show-crawler-runtime/executor/ValueStep.h
        include/tv-show-crawler-runtime/executor/TransformStep.h
        include/tv-show-crawler-runtime/executor/FetchStep.h
        src/tv-show-crawler-runtime/TvShowCrawlerRuntime.cpp
        src/tv-show-crawler-runtime/CrawlerSerializer.cpp
        src/tv-show-crawler-runtime/model/CrawlerStep.cpp
        src/tv-show-crawler-runtime/model/Crawler.cpp
        src/tv-show-crawler-runtime/model/TvShowCrawler.cpp
        src/tv-show-crawler-runtime/model/CrawlerStepType.cpp
        src/tv-show-crawler-runtime/executor/CrawlerExecutor.cpp
        src/tv-show-crawler-runtime/executor/ValueStep.cpp
        src/tv-show-crawler-runtime/executor/TransformStep.cpp
        src/tv-show-crawler-runtime/executor/FetchStep.cpp
        src/tv-show-crawler-runtime/executor/RegExpStep.cpp
        include/tv-show-crawler-editor/TvShowCrawlerEditor.h
        include/tv-show-crawler-editor/TvShowCrawlerBuilder.h
        include/tv-show-crawler-editor/CrawlerBuilder.h
        src/tv-show-crawler-editor/TvShowCrawlerEditor.cpp
        src/tv-show-crawler-editor/TvShowCrawlerBuilder.cpp
        src/tv-show-crawler-editor/CrawlerBuilder.cpp
        include/tv-show-viewer/TvShowViewer.h
        src/tv-show-viewer/TvShowViewer.cpp
        include/tv-show-player/TvShowPlayer.h
        include/tv-show-player/Player.h
        src/tv-show-player/TvShowPlayer.cpp
        src/tv-show-player/Player.cpp)
target_include_directories(Core PUBLIC include PUBLIC lib/json)

add_library(CoreTestMocks INTERFACE)
target_include_directories(CoreTestMocks INTERFACE test/mocks)
target_link_libraries(CoreTestMocks INTERFACE Core gmock)

add_executable(CoreTest
        test/tv-show-storage/TvShowTest.cpp
        test/tv-show-storage/LastWatchDateTest.cpp
        test/tv-show-storage/ListPageTest.cpp
        test/tv-show-storage/BigListProxyTest.cpp
        test/tv-show-storage/EpisodeTest.cpp
        test/tv-show-storage/WatchProgressTest.cpp
        test/tv-show-crawler-log-storage/CrawlLogEntryTest.cpp
        test/tv-show-crawler-runtime/model/CrawlerStepTest.cpp
        test/tv-show-crawler-runtime/model/CrawlerTest.cpp
        test/tv-show-crawler-runtime/model/TvShowCrawlerTest.cpp
        test/tv-show-crawler-runtime/model/CrawlerStepTypeTest.cpp
        test/tv-show-crawler-runtime/TvShowCrawlerRuntimeTest.cpp
        test/tv-show-crawler-editor/TvShowCrawlerEditorTest.cpp
        test/tv-show-viewer/TvShowViewerTest.cpp
        test/tv-show-player/TvShowPlayerTest.cpp)
target_link_libraries(CoreTest Core CoreTestMocks gtest gmock gtest_main)