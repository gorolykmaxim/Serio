add_subdirectory(lib/googletest)
add_subdirectory(lib/NFHTTP)
add_subdirectory(lib/SQLiteCpp)
add_subdirectory(lib/iconv-lite)

add_library(mjs STATIC lib/mjs/mjs.c)
target_include_directories(mjs PUBLIC lib/mjs)

add_library(core STATIC src/cache/cache.cpp include/config.h src/config.cpp)
target_include_directories(core PUBLIC include PUBLIC lib/SQLiteCpp/include PUBLIC lib/NFHTTP/include)
target_link_libraries(core SQLiteCpp NFHTTP nlohmann_json mjs iconvlite)

add_executable(core-test test/cache/cache-test.cpp test/config-test.cpp)
target_include_directories(core-test PRIVATE test/mocks)
target_link_libraries(core-test core gtest gmock gtest_main)