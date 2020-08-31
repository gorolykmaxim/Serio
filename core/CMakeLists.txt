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
        include/tv-show-crawler-storage/TvShowCrawlerStorage.h)
target_include_directories(Core PUBLIC include PRIVATE lib/json)

add_executable(CoreTest
        test/tv-show-storage/TvShowTest.cpp
        test/tv-show-storage/LastWatchDateTest.cpp
        test/tv-show-storage/ListPageTest.cpp
        test/tv-show-storage/BigListProxyTest.cpp
        test/tv-show-storage/EpisodeTest.cpp)
target_link_libraries(CoreTest Core gtest gmock gtest_main)