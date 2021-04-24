add_subdirectory(lib/googletest)
add_subdirectory(lib/NFHTTP)
add_subdirectory(lib/SQLiteCpp)
add_subdirectory(lib/iconv-lite)

add_library(mjs STATIC lib/mjs/mjs.c)
target_include_directories(mjs PUBLIC lib/mjs)

add_library(Core STATIC)
target_include_directories(Core PUBLIC include PUBLIC lib/SQLiteCpp/include PUBLIC lib/NFHTTP/include)
target_link_libraries(Core SQLiteCpp NFHTTP nlohmann_json mjs iconvlite)

add_executable(CoreTest)
target_include_directories(CoreTest PRIVATE test/mocks)
target_link_libraries(CoreTest Core gtest gmock gtest_main)