set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Quick Concurrent Sql Network Test REQUIRED)
qt5_add_resources(RESOURCES resources/qml.qrc)

add_library(SerioLib STATIC
        src/Application.h
        src/Application.cpp
        src/storage/DatabaseStorage.cpp
        src/storage/DatabaseStorage.h
        src/user-interface/model/TvShowListModel.cpp
        src/user-interface/model/TvShowListModel.h
        src/user-interface/view-model/AllTvShowsViewModel.cpp
        src/user-interface/view-model/AllTvShowsViewModel.h
        src/concurrency/TaskExecutor.h
        src/network/QHttpClient.cpp
        src/network/QHttpClient.h
        src/network/FetchRequest.cpp
        src/network/FetchRequest.h
        src/storage/DatabaseTvShowStorage.cpp
        src/storage/DatabaseTvShowStorage.h
        src/storage/DatabaseTvShowCrawlerStorage.cpp
        src/storage/DatabaseTvShowCrawlerStorage.h
        src/user-interface/UserInterface.cpp
        src/user-interface/UserInterface.h)
target_include_directories(SerioLib PUBLIC src)
target_link_libraries(SerioLib Qt5::Quick Qt5::Concurrent Qt5::Sql Qt5::Network Core)

add_executable(Serio ${RESOURCES} src/main.cpp)
target_link_libraries(Serio SerioLib)

add_executable(SerioTest test/storage/DatabaseTvShowStorageTest.cpp
        test/user-interface/model/TvShowListModelTest.cpp
        test/storage/DatabaseTvShowCrawlerStorageTest.cpp)
target_link_libraries(SerioTest SerioLib gtest gtest_main Qt5::Test)