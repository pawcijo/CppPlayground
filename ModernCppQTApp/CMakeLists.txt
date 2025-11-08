
project(ModernCppQTApp VERSION 1.0)

#Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia Quick QuickWidgets)
find_package(Vulkan REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

#Enable moc for Qt6 so it can generate the necessary files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#MacOS sources 


# Project name and version
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/periodictablewindow.cpp
)

if(APPLE)
    list(APPEND SOURCES src/MacOS/MetalItem.cpp src/MacOS/MetalSphereItem.cpp src/MacOS/MetalSphereNode.cpp)
endif()

# Define header files
set(HEADERS
    src/mainwindow.h
    src/periodictablewindow.h
)

if(APPLE)
    list(APPEND HEADERS src/MacOS/MetalItem.h src/MacOS/MetalSphereItem.h src/MacOS/MetalSphereNode.h)
endif()

# Define UI files
set(UI_FILES
    src/mainwindow.ui
    src/periodictablewindow.ui
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
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PlaygroundLib
    Vulkan::Vulkan
    Qt6::Multimedia
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickWidgets
)
