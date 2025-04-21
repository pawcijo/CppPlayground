#include <fcntl.h>
#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>

#include "DemosFactory/DemoFactory.hpp"  // Include the necessary header for DemoFactory

TEST(DemoFactoryTestTest, BasicAssertions)
{
    // Test test :)
    EXPECT_EQ(1 + 1, 2);
}

TEST(DemoFactoryTest, NoThrowOnCreateDemo)
{
    // Test all demo types to ensure no exceptions are thrown
    for (int i = 0; i < static_cast<int>(DemoType::Count); ++i)
    {
        try
        {
            auto demo = DemoFactory::createDemo(static_cast<DemoType>(i));
        }
        catch (const std::exception& e)
        {
            std::cerr << "Caught an exception: " << e.what() << std::endl;
            FAIL() << "Test failed due to exception: " << e.what();  // Fail the test explicitly
        }
    }
}

struct OutputSilencer {
    int stdoutFd;
    int devNullFd;

    OutputSilencer() {
        // Save original stdout (to restore later)
        stdoutFd = dup(fileno(stdout));  

        // Redirect stdout (printf) to /dev/null
        devNullFd = open("/dev/null", O_WRONLY);
        dup2(devNullFd, fileno(stdout));  // Redirect stdout to /dev/null
    }

    ~OutputSilencer() {
        // Restore stdout (printf)
        fflush(stdout);
        dup2(stdoutFd, fileno(stdout));
        close(devNullFd);
        close(stdoutFd);
    }
};

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
     //Uncomment to silence stdout
     //OutputSilencer silence; 
    return RUN_ALL_TESTS(); 
}