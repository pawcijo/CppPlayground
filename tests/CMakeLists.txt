cmake_minimum_required(VERSION 3.10)
project(MyTests)

set(CMAKE_CXX_STANDARD 20)

# Add GoogleTest submodule
add_subdirectory(googletest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories for the project
include_directories(${CMAKE_SOURCE_DIR}/src/Common)
include_directories(${CMAKE_SOURCE_DIR}/src/Demos/CPP17)
include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)

# Add the test executable
file(GLOB_RECURSE SOURCES "../src/*.cpp")
add_executable(runTests main_test.cpp ${SOURCES})

# Enable testing
enable_testing()
add_test(NAME MyTests COMMAND runTests)

# Set DYLD_LIBRARY_PATH to locate dynamic libraries on macOS
if(APPLE)
    set_property(TEST MyTests PROPERTY ENVIRONMENT "DYLD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib")
endif()




# Add a custom target to run tests after building

add_custom_target(check
    ALL
    DEPENDS runTests
    COMMAND ctest --output-on-failure --verbose
    COMMENT "Running tests"
)

# Link GoogleTest to your test executable
target_link_libraries(runTests GTest::gtest GTest::gtest_main)
