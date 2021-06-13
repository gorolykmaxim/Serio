add_subdirectory(lib/cxxopts)

add_executable(crdb src/main.cpp)
target_link_libraries(crdb core cxxopts)