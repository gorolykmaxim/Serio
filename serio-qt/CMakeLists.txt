set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Quick REQUIRED)
qt5_add_resources(RESOURCES resources/qml.qrc)

add_executable(Serio ${RESOURCES}
        src/main.cpp
        src/Application.cpp
        src/Application.h)
target_link_libraries(Serio Qt5::Quick Core)