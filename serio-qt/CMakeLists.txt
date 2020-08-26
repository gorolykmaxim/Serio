set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Quick Concurrent Sql Test REQUIRED)
qt5_add_resources(RESOURCES resources/qml.qrc)

add_library(SerioLib STATIC
        src/Application.h
        src/Application.cpp
        src/storage/TvShowStorage.cpp
        src/storage/TvShowStorage.h
        src/storage/StorageError.cpp
        src/storage/StorageError.h
        src/model/TvShowListModel.cpp
        src/model/TvShowListModel.h
        src/view-model/AllTvShowsViewModel.cpp
        src/view-model/AllTvShowsViewModel.h
        src/concurrency/TaskExecutor.h)
target_include_directories(SerioLib PUBLIC src)
target_link_libraries(SerioLib Qt5::Quick Qt5::Concurrent Qt5::Sql Core)

add_executable(Serio ${RESOURCES} src/main.cpp)
target_link_libraries(Serio SerioLib)

add_executable(SerioTest test/storage/TvShowStorageTest.cpp
        test/model/TvShowListModelTest.cpp)
target_link_libraries(SerioTest SerioLib gtest gtest_main Qt5::Test)