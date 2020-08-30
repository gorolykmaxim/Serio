add_library(Core STATIC
        include/tv-shows/TvShowsFacade.h
        include/tv-shows/TvShow.h
        include/tv-shows/LastWatchDate.h
        src/tv-shows/TvShow.cpp
        src/tv-shows/LastWatchDate.cpp
        include/tv-shows/ListPage.h
        include/tv-shows/BigListProxy.h
        include/http-client/HttpClient.h)
target_include_directories(Core PUBLIC include PRIVATE lib/json)

add_executable(CoreTest
        test/tv-shows/TvShowTest.cpp
        test/tv-shows/LastWatchDateTest.cpp
        test/tv-shows/ListPageTest.cpp
        test/tv-shows/BigListProxyTest.cpp)
target_link_libraries(CoreTest Core gtest gmock gtest_main)