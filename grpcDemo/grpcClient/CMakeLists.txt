add_executable(grpcClient client.cpp)

target_link_libraries(grpcClient
    PRIVATE
        proto_lib
        gRPC::grpc++
)

target_include_directories(grpcClient
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/../generated
)
