set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Quick Concurrent Sql Network Test REQUIRED)
qt5_add_resources(RESOURCES resources/qml.qrc resources/lib/QtMaterialDesignIcons/resources/QtMaterialIcons.qrc)

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
        src/task-executor/QTaskExecutor.cpp
        src/http-client/QHttpClient.cpp
        src/http-client/QHttpClient.h
        src/http-client/FetchRequest.cpp
        src/http-client/FetchRequest.h
        src/http-client/CachingNetworkAccessManagerFactory.cpp
        src/http-client/CachingNetworkAccessManagerFactory.h
        src/user-interface/action/ActionType.h
        src/user-interface/action/ActionRouter.cpp
        src/user-interface/action/ActionRouter.h
        src/user-interface/model/pageable-list/TvShowListModel.cpp
        src/user-interface/model/pageable-list/TvShowListModel.h
        src/user-interface/model/pageable-list/EpisodeListModel.cpp
        src/user-interface/model/pageable-list/EpisodeListModel.h
        src/user-interface/model/pageable-list/InvalidListRole.cpp
        src/user-interface/model/pageable-list/InvalidListRole.h
        src/user-interface/model/pageable-list/AbstractListModelServant.h
        src/user-interface/model/pageable-list/AbstractListModel.cpp
        src/user-interface/model/pageable-list/AbstractListModel.h
        src/user-interface/model/TileModel.cpp
        src/user-interface/model/TileModel.h
        src/user-interface/model/RadioButtonModel.cpp
        src/user-interface/model/RadioButtonModel.h
        src/user-interface/model/TextFieldModel.cpp
        src/user-interface/model/TextFieldModel.h
        src/user-interface/model/ListModel.h
        src/user-interface/view-model/AllTvShowsViewModel.cpp
        src/user-interface/view-model/AllTvShowsViewModel.h
        src/user-interface/view-model/TvShowCrawlerEditorViewModel.cpp
        src/user-interface/view-model/TvShowCrawlerEditorViewModel.h
        src/user-interface/view-model/CrawlerEditorViewModel.cpp
        src/user-interface/view-model/CrawlerEditorViewModel.h
        src/user-interface/view-model/ViewModel.h
        src/user-interface/view-model/CrawlerStepEditorViewModel.cpp
        src/user-interface/view-model/CrawlerStepEditorViewModel.h
        src/user-interface/view-model/ErrorViewModel.cpp
        src/user-interface/view-model/ErrorViewModel.h
        src/user-interface/view-model/CrawlLogViewModel.cpp
        src/user-interface/view-model/CrawlLogViewModel.h
        src/user-interface/view-model/TvShowViewModel.cpp
        src/user-interface/view-model/TvShowViewModel.h
        src/user-interface/view-model/ViewModel.cpp
        src/user-interface/UserInterface.cpp
        src/user-interface/UserInterface.h
        src/user-interface/StackOfViews.cpp
        src/user-interface/StackOfViews.h
        src/user-interface/ViewNames.h)
target_include_directories(SerioLib PUBLIC src)
target_link_libraries(SerioLib Qt5::Quick Qt5::Concurrent Qt5::Sql Qt5::Network Core)

add_executable(Serio ${RESOURCES} src/main.cpp)
target_link_libraries(Serio SerioLib)

add_executable(SerioTest
        test/user-interface/model/pageable-list/TvShowListModelTest.cpp
        test/user-interface/model/pageable-list/EpisodeListModelTest.cpp
        test/database-storage/DatabaseTvShowStorageTest.cpp
        test/database-storage/DatabaseTvShowCrawlerStorageTest.cpp
        test/user-interface/view-model/AllTvShowsViewModelTest.cpp
        test/user-interface/view-model/TvShowCrawlerEditorViewModelTest.cpp
        test/user-interface/view-model/CrawlerEditorViewModelTest.cpp
        test/user-interface/view-model/CrawlerStepEditorViewModelTest.cpp
        test/user-interface/view-model/ErrorViewModelTest.cpp
        test/user-interface/view-model/StackOfViewsTest.cpp
        test/user-interface/view-model/CrawlLogViewModelTest.cpp
        test/user-interface/view-model/TvShowViewModelTest.cpp)
target_include_directories(SerioTest PUBLIC test/mocks)
target_link_libraries(SerioTest SerioLib CoreTestMocks gtest gmock gtest_main Qt5::Test)