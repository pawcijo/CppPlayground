#include <gtest/gtest.h>
#include "Common/DemoFactory.hpp" // Include the necessary header for DemoFactory

#include <iostream>
#include <cstdio>
#include <fstream>
#include <fcntl.h>

TEST(DemoFactoryTest, BasicAssertions) {
    //Test test :)
    EXPECT_EQ(1 + 1, 2);
}


TEST(DemoFactoryTest, NoThrowOnCreateDemo) {
    // Test all demo types to ensure no exceptions are thrown
    for (int i = 0; i < static_cast<int>(Demos::Count); ++i) {
        EXPECT_NO_THROW({
            auto demo = DemoFactory::createDemo(static_cast<Demos>(i));
            if (demo) {
                demo->PrintName();
                demo->PrintNotes();
                demo->ShowExample();
                demo->ShowDemo();
            }
        });
    }
}

struct OutputSilencer {
    std::streambuf* coutBuffer;
    int stdoutFd;
    int devNullFd;

    OutputSilencer() {
        // Redirect std::cout
        coutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(nullptr);

        // Redirect stdout (printf)
        stdoutFd = dup(fileno(stdout));  // Save original stdout
        devNullFd = open("/dev/null", O_WRONLY);
        dup2(devNullFd, fileno(stdout));  // Redirect stdout to /dev/null
    }

    ~OutputSilencer() {
        // Restore std::cout
        std::cout.rdbuf(coutBuffer);

        // Restore stdout (printf)
        fflush(stdout);
        dup2(stdoutFd, fileno(stdout)); 
        close(devNullFd);
        close(stdoutFd);
    }
};


int main(int argc, char **argv) {

   
    ::testing::InitGoogleTest(&argc, argv);
     OutputSilencer silence;
    return RUN_ALL_TESTS();
}