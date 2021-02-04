add_subdirectory(lib/googletest)
add_subdirectory(lib/NFHTTP)
add_subdirectory(lib/SQLiteCpp)
add_subdirectory(lib/iconv-lite)

add_library(mjs STATIC lib/mjs/mjs.c)
target_include_directories(mjs PUBLIC lib/mjs)

add_library(Core STATIC
        src/cache/Cache.cpp
        src/config/Config.cpp
        src/config/HttpClientConfig.cpp
        src/config/ConfigSource.cpp
        src/config/ConfigStructure.cpp
        src/config/TvShowCrawlerConfig.cpp
        src/config/SearchCrawlerConfig.cpp
        src/config/CategoryCrawlerConfig.cpp
        src/config/SuggestionsCrawlerConfig.cpp
        src/config/PersistentProperties.cpp
        src/http-client/HttpClient.cpp
        src/http-client/HttpRequest.cpp
        src/http-client/HttpResponse.cpp
        src/crawler-runtime/CrawlerRuntime.cpp
        src/crawler-runtime/CrawlerExecution.cpp
        src/crawler-runtime/RegExpSystem.cpp
        src/crawler-runtime/JsObject.cpp
        src/crawler-runtime/ResultFetchSystem.cpp
        src/crawler-runtime/CrawlerExecutionSystem.cpp
        src/crawler-runtime/CrawlerArgumentSystem.cpp
        src/crawler-runtime/HttpRequestSystem.cpp
        src/crawler-runtime/LoggingSystem.cpp
        src/crawler-runtime/ProfilerSystem.cpp
        src/localization/Localization.cpp)
target_include_directories(Core PUBLIC include PUBLIC lib/SQLiteCpp/include PUBLIC lib/NFHTTP/include)
target_link_libraries(Core SQLiteCpp NFHTTP nlohmann_json mjs iconvlite)

add_executable(CoreTest
        test/cache/CacheTest.cpp
        test/config/ConfigTest.cpp
        test/http-client/HttpClientTest.cpp
        test/crawler-runtime/CrawlerRuntimeTest.cpp
        test/localization/LocalizationTest.cpp)
target_include_directories(CoreTest PRIVATE test/mocks)
target_link_libraries(CoreTest Core gtest gmock gtest_main)