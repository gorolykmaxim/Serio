add_subdirectory(lib/googletest)
add_subdirectory(lib/NFHTTP)
add_subdirectory(lib/SQLiteCpp)

add_library(Core STATIC
        src/cache/Cache.cpp
        src/config/Config.cpp
        src/config/HttpClientConfig.cpp
        src/config/ConfigSource.cpp
        src/config/ConfigStructure.cpp)
target_include_directories(Core PUBLIC include PUBLIC lib/SQLiteCpp/include PUBLIC lib/NFHTTP/include)
target_link_libraries(Core SQLiteCpp NFHTTP nlohmann_json)

add_executable(CoreTest test/cache/CacheTest.cpp test/config/ConfigTest.cpp)
target_include_directories(CoreTest PRIVATE test/mocks)
target_link_libraries(CoreTest Core gtest gmock gtest_main)