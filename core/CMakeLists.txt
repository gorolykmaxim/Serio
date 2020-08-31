add_library(Core STATIC
        include/tv-show-storage/TvShowStorage.h
        include/tv-show-storage/TvShow.h
        include/tv-show-storage/LastWatchDate.h
        src/tv-shows/TvShow.cpp
        src/tv-shows/LastWatchDate.cpp
        include/tv-show-storage/ListPage.h
        include/tv-show-storage/BigListProxy.h
        include/http-client/HttpClient.h
        include/tv-show-crawler-storage/TvShowCrawlerStorage.h)
target_include_directories(Core PUBLIC include PRIVATE lib/json)

add_executable(CoreTest
        test/tv-shows/TvShowTest.cpp
        test/tv-shows/LastWatchDateTest.cpp
        test/tv-shows/ListPageTest.cpp
        test/tv-shows/BigListProxyTest.cpp)
target_link_libraries(CoreTest Core gtest gmock gtest_main)