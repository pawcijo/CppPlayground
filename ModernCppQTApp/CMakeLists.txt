
project(ModernCppQTApp VERSION 1.0)

#Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Enable moc for Qt6 so it can generate the necessary files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Project name and version
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
)

# Define header files
set(HEADERS
    src/mainwindow.h
)

# Define UI files
set(UI_FILES
    src/mainwindow.ui
)

# Add Qt resources
qt6_add_resources(RESOURCES resources.qrc)


# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCES}
)

#Include directories for static library
target_include_directories(${PROJECT_NAME} PUBLIC
    ../PlayGroundLib
)

target_link_libraries(${PROJECT_NAME}
    PlaygroundLib
    Qt6::Multimedia
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
