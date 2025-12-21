#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterClient {
public:
    explicit GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}

    std::string SayHello(const std::string& user) {
        HelloRequest request;
        request.set_name(user);

        HelloReply reply;
        ClientContext context;

        Status status = stub_->SayHello(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            std::cerr << "RPC failed: "
                      << status.error_code() << ": "
                      << status.error_message() << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string target = "localhost:50051";
    if (argc > 1) {
        target = argv[1];
    }

    GreeterClient client(
        grpc::CreateChannel(target, grpc::InsecureChannelCredentials())
    );

    std::string user = "World";
    std::string reply = client.SayHello(user);
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}
