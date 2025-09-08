#include <cstdint>
#include <fcntl.h>
#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>

#include "DemosFactory/DemoFactory.hpp" // Include the necessary header for DemoFactory

TEST(DemoFactoryTestTest, BasicAssertions)
{
  // Test test :)
  EXPECT_EQ(1 + 1, 2);
}

TEST(LongestCharSequenceTest, LongestCharSequence)
{
  LongestCharSequence demo;
  EXPECT_EQ(demo.FindLongestCharSequence("aaabbcddddeeea"), 4);
  EXPECT_EQ(demo.FindLongestCharSequence("abcd"), 1);
  EXPECT_EQ(demo.FindLongestCharSequence("abcdabcd"), 1);
  EXPECT_EQ(demo.FindLongestCharSequence("abbbbcdaaaaaa"), 6);
  EXPECT_EQ(demo.FindLongestCharSequence("abcddddbbddd"), 4);
  EXPECT_EQ(demo.FindLongestCharSequence("ee1eee1eee1"), 3);
  EXPECT_EQ(demo.FindLongestCharSequence(""), 0);
  EXPECT_EQ(demo.FindLongestCharSequence("a"), 1);
  EXPECT_EQ(demo.FindLongestCharSequence("aaaaaa"), 6);

  // Edge cases
  EXPECT_EQ(demo.FindLongestCharSequence("aabbbccccddddddeee"), 6);
  EXPECT_EQ(demo.FindLongestCharSequence("xyzzyx"), 2);
  EXPECT_EQ(demo.FindLongestCharSequence("1112233333444"), 5);
}

TEST(StringParser, StringParserTest)
{

  long long maxSupportedValue = INT_FAST64_MAX;
  long long minSupportedValue = INT_FAST64_MIN;

  EXPECT_EQ(StringParser::ParseString("2137"), 2137);
  EXPECT_EQ(StringParser::ParseString("222"), 222);
  EXPECT_EQ(StringParser::ParseString("+1"), 1);
  EXPECT_EQ(StringParser::ParseString("-1"), -1);
  EXPECT_EQ(StringParser::ParseString("-1222"), -1222);
  EXPECT_EQ(StringParser::ParseString("12345678900"), 12345678900);
  EXPECT_EQ(StringParser::ParseString("12345678900"), 12345678900);
  EXPECT_EQ(StringParser::ParseString("9223372036854775807"),
            maxSupportedValue);
  EXPECT_EQ(StringParser::ParseString("-9223372036854775808"),
            minSupportedValue);

  // buffer overflow
  EXPECT_EQ(StringParser::ParseString("9223372036854775808"),
            minSupportedValue);
  EXPECT_EQ(StringParser::ParseString("-9223372036854775809"),
            maxSupportedValue);
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
      FAIL() << "Test failed due to exception: "
             << e.what(); // Fail the test explicitly
    }
  }
}

struct OutputSilencer
{
  int stdoutFd;
  int devNullFd;

  OutputSilencer()
  {
    // Save original stdout (to restore later)
    stdoutFd = dup(fileno(stdout));

    // Redirect stdout (printf) to /dev/null
    devNullFd = open("/dev/null", O_WRONLY);
    dup2(devNullFd, fileno(stdout)); // Redirect stdout to /dev/null
  }

  ~OutputSilencer()
  {
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
  // Uncomment to silence stdout
  // OutputSilencer silence;
  return RUN_ALL_TESTS();
}