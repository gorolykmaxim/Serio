add_library(Core STATIC
        include/HelloWorld.h
        src/HelloWorld.cpp)
target_include_directories(Core PUBLIC include)

add_executable(CoreTest test/HelloWorldTest.cpp)
target_link_libraries(CoreTest Core gtest gmock gtest_main)