add_subdirectory(lib/cxxopts)

add_executable(crdb src/main.cpp
        src/HardcodedConfig.cpp
        src/Arguments.cpp)
target_link_libraries(crdb Core cxxopts)