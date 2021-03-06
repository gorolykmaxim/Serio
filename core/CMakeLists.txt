add_subdirectory(lib/googletest)
add_subdirectory(lib/NFHTTP)
add_subdirectory(lib/SQLiteCpp)
add_subdirectory(lib/iconv-lite)
add_subdirectory(lib/concurrentqueue)

add_library(mjs STATIC lib/mjs/mjs.c)
target_include_directories(mjs PUBLIC lib/mjs)

add_library(core STATIC
        include/config.h
        include/http.h
        include/queue.h
        include/task.h
        include/crawler.h
        include/id.h
        include/core.h
        include/ui.h
        include/localization.h
        src/config.cpp
        src/http.cpp
        src/task.cpp
        src/crawler.cpp
        src/id.cpp
        src/core.cpp
        src/ui.cpp
        src/localization/localization.cpp
        src/localization/english.h
        src/localization/russian.h)
target_include_directories(core PUBLIC include PUBLIC lib/SQLiteCpp/include PUBLIC lib/NFHTTP/include PUBLIC lib/concurrentqueue)
target_link_libraries(core SQLiteCpp NFHTTP nlohmann_json mjs iconvlite)

add_executable(core-test
        test/lib/test-lib.cpp
        test/lib/test-lib.h
        test/lib/test-data.h
        test/lib/test-data-en.h
        test/lib/test-data-ru.h
        test/lib/http-mock.h
        test/lib/serio-test.h
        test/functional-test.cpp)
target_include_directories(core-test PRIVATE test/mocks)
target_link_libraries(core-test core gtest gmock gtest_main)