cmake_minimum_required(VERSION 3.16)

project(GRPCDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
find_package(gRPC REQUIRED)

# ---------------------------------------------------------------------------
# Proto configuration (ABSOLUTE PATHS – IMPORTANT)
# ---------------------------------------------------------------------------
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(PROTO_FILE ${PROTO_DIR}/helloworld.proto)

set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(GENERATED_SRCS
    ${GENERATED_DIR}/helloworld.pb.cc
    ${GENERATED_DIR}/helloworld.grpc.pb.cc
)

set(GENERATED_HDRS
    ${GENERATED_DIR}/helloworld.pb.h
    ${GENERATED_DIR}/helloworld.grpc.pb.h
)

# ---------------------------------------------------------------------------
# Proto generation
# ---------------------------------------------------------------------------
add_custom_command(
    OUTPUT
        ${GENERATED_SRCS}
        ${GENERATED_HDRS}

    COMMAND protobuf::protoc
    ARGS
        --proto_path=${PROTO_DIR}
        --cpp_out=${GENERATED_DIR}
        --grpc_out=${GENERATED_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${PROTO_FILE}

    DEPENDS
        ${PROTO_FILE}

    COMMENT "Generating gRPC sources from helloworld.proto"
)

# ---------------------------------------------------------------------------
# Proto library
# ---------------------------------------------------------------------------
add_library(proto_lib
    ${GENERATED_SRCS}
    ${GENERATED_HDRS}
)

target_include_directories(proto_lib
    PUBLIC
        ${GENERATED_DIR}
)

target_link_libraries(proto_lib
    PUBLIC
        gRPC::grpc++
        protobuf::libprotobuf
)

# Ensure macOS uses libc++
# Set macOS SDK and libc++
if(APPLE)
    # Get SDK path
    execute_process(
        COMMAND xcrun --show-sdk-path
        OUTPUT_VARIABLE MACOS_SDK
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Use libc++ and the SDK
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -isysroot ${MACOS_SDK}")
    set(CMAKE_OSX_SYSROOT ${MACOS_SDK})
endif()

# ---------------------------------------------------------------------------
# Subprojects
# ---------------------------------------------------------------------------
add_subdirectory(grpcServer)
add_subdirectory(grpcClient)
