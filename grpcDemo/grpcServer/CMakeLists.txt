add_executable(grpcServer server.cpp)

target_link_libraries(grpcServer
    PRIVATE
        proto_lib
        gRPC::grpc++
)

target_include_directories(grpcServer
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/../generated
)
