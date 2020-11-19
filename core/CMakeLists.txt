add_subdirectory(lib/googletest)
add_subdirectory(lib/NFHTTP)
add_subdirectory(lib/SQLiteCpp)

add_library(Core STATIC src/cache/Cache.cpp)
target_include_directories(Core PUBLIC include PUBLIC lib/SQLiteCpp/include)
target_link_libraries(Core SQLiteCpp)

add_executable(CoreTest test/cache/CacheTest.cpp)
target_link_libraries(CoreTest Core gtest gmock gtest_main)