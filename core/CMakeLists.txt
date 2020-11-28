add_subdirectory(lib/googletest)
add_subdirectory(lib/NFHTTP)
add_subdirectory(lib/SQLiteCpp)

add_library(mjs STATIC lib/mjs/mjs.c)
target_include_directories(mjs PUBLIC lib/mjs)

add_library(Core STATIC
        src/cache/Cache.cpp
        src/config/Config.cpp
        src/config/HttpClientConfig.cpp
        src/config/ConfigSource.cpp
        src/config/ConfigStructure.cpp
        src/config/TvShowCrawlerConfig.cpp
        src/caching-http-client/CachingHttpClient.cpp
        src/caching-http-client/HttpRequest.cpp
        src/config/SearchCrawlerConfig.cpp
        src/config/CategoryCrawlerConfig.cpp
        src/crawler-http-client/CrawlerHttpClient.cpp
        src/crawler-runtime/CrawlerRuntime.cpp
        src/crawler-runtime/CrawlerExecution.cpp
        src/crawler-runtime/RegExpSystem.cpp
        src/crawler-runtime/JsObject.cpp
        src/crawler-runtime/ResultFetchSystem.cpp
        src/crawler-runtime/CrawlerExecutionSystem.cpp
        src/crawler-runtime/CrawlerArgumentSystem.cpp
        src/crawler-runtime/HttpRequestSystem.cpp
        src/crawler-runtime/LoggingSystem.cpp)
target_include_directories(Core PUBLIC include PUBLIC lib/SQLiteCpp/include PUBLIC lib/NFHTTP/include)
target_link_libraries(Core SQLiteCpp NFHTTP nlohmann_json mjs)

add_executable(CoreTest
        test/cache/CacheTest.cpp
        test/config/ConfigTest.cpp
        test/caching-http-client/CachingHttpClientTest.cpp
        test/crawler-http-client/CrawlerHttpClientTest.cpp
        test/crawler-runtime/CrawlerRuntimeTest.cpp)
target_include_directories(CoreTest PRIVATE test/mocks)
target_link_libraries(CoreTest Core gtest gmock gtest_main)