set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Quick Concurrent Sql Network Test REQUIRED)
qt5_add_resources(RESOURCES resources/qml.qrc)

add_library(SerioLib STATIC
        src/Application.h
        src/Application.cpp
        src/database-storage/DatabaseStorage.cpp
        src/database-storage/DatabaseStorage.h
        src/database-storage/DatabaseTvShowStorage.cpp
        src/database-storage/DatabaseTvShowStorage.h
        src/database-storage/DatabaseTvShowCrawlerStorage.cpp
        src/database-storage/DatabaseTvShowCrawlerStorage.h
        src/task-executor/QTaskExecutor.h
        src/http-client/QHttpClient.cpp
        src/http-client/QHttpClient.h
        src/http-client/FetchRequest.cpp
        src/http-client/FetchRequest.h
        src/user-interface/model/TvShowListModel.cpp
        src/user-interface/model/TvShowListModel.h
        src/user-interface/view-model/AllTvShowsViewModel.cpp
        src/user-interface/view-model/AllTvShowsViewModel.h
        src/user-interface/UserInterface.cpp
        src/user-interface/UserInterface.h)
target_include_directories(SerioLib PUBLIC src)
target_link_libraries(SerioLib Qt5::Quick Qt5::Concurrent Qt5::Sql Qt5::Network Core)

add_executable(Serio ${RESOURCES} src/main.cpp)
target_link_libraries(Serio SerioLib)

add_executable(SerioTest
        test/user-interface/model/TvShowListModelTest.cpp
        test/database-storage/DatabaseTvShowStorageTest.cpp
        test/database-storage/DatabaseTvShowCrawlerStorageTest.cpp)
target_link_libraries(SerioTest SerioLib gtest gtest_main Qt5::Test)