add_subdirectory(lib/cxxopts)

add_executable(crdb)
target_link_libraries(crdb Core cxxopts)