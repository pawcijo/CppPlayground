#include <cstdint>
#include <fcntl.h>
#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>
#include <format>
#include <iterator>
#include <string>
#include <chrono>

#include "Common/MyUniquePtr.hpp"
#include "DemosFactory/DemoFactory.hpp" // Include the necessary header for DemoFactory

#include "Common/MySharedPtr.hpp"
#include "Common/MySharedPtr.hpp"
#include <type_traits>
TEST(DemoFactoryTestTest, BasicAssertions)
{
  // Test test :)
  EXPECT_EQ(1 + 1, 2);
}

TEST(DemoFactoryTestTest, MyUniquePtrTest)
{
  static_assert(!std::is_copy_constructible_v<PlaygroundLib::MyUniquePtr<int>>,
                "Should not be copy constructible");
  static_assert(!std::is_copy_assignable_v<PlaygroundLib::MyUniquePtr<int>>,
                "Should not be copy assignable");

  static_assert(std::is_move_constructible_v<PlaygroundLib::MyUniquePtr<int>>,
                "Should be move constructible");
  static_assert(std::is_move_assignable_v<PlaygroundLib::MyUniquePtr<int>>,
                "Should be move assignable");
  }


TEST(DemoFactoryTestTest, MySharedPtrTest)
{
  static_assert(std::is_copy_constructible_v<PlaygroundLib::MySharedPtr<int>>,
                "Should be copy constructible");
  static_assert(std::is_copy_assignable_v<PlaygroundLib::MySharedPtr<int>>,
                "Should be copy assignable");

  static_assert(std::is_move_constructible_v<PlaygroundLib::MySharedPtr<int>>,
                "Should be move constructible");
  static_assert(std::is_move_assignable_v<PlaygroundLib::MySharedPtr<int>>,
                "Should be move assignable");
 
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


TEST(FormatMemoryTest, CompareFormatAndFormatTo)
{
    constexpr auto formatString = "The answer is {}. This is a longer string to test heap allocations.";
    constexpr int argument = 42;

    // ------------------------------------------------
    // A: std::format()
    // ------------------------------------------------
    auto startFormat = std::chrono::steady_clock::now();

    std::string formatted;

    for (int i = 0; i < 10000; ++i)
    {
        formatted = std::format(formatString, argument);
    }

    auto endFormat = std::chrono::steady_clock::now();

    auto formatTime =
        std::chrono::duration_cast<std::chrono::microseconds>(
            endFormat - startFormat).count();

    // ------------------------------------------------
    // B: std::format_to() without reserve()
    // ------------------------------------------------
    auto startFormatTo = std::chrono::steady_clock::now();

    std::string buffer;

    for (int i = 0; i < 10000; ++i)
    {
        buffer.clear();

        std::format_to(
            std::back_inserter(buffer),
            formatString,
            argument
        );
    }

    auto endFormatTo = std::chrono::steady_clock::now();

    auto formatToTime =
        std::chrono::duration_cast<std::chrono::microseconds>(
            endFormatTo - startFormatTo).count();

    // ------------------------------------------------
    // C: std::format_to() with reserve()
    // ------------------------------------------------
    std::string reservedBuffer;
    reservedBuffer.reserve(128);

    auto startReserved = std::chrono::steady_clock::now();

    for (int i = 0; i < 10000; ++i)
    {
        reservedBuffer.clear();

        std::format_to(
            std::back_inserter(reservedBuffer),
            formatString,
            argument
        );
    }

    auto endReserved = std::chrono::steady_clock::now();

    auto reservedTime =
        std::chrono::duration_cast<std::chrono::microseconds>(
            endReserved - startReserved).count();

    // ------------------------------------------------
    // Verify identical results
    // ------------------------------------------------
    EXPECT_EQ(formatted, buffer);
    EXPECT_EQ(formatted, reservedBuffer);

    // Verify reserved capacity was sufficient
    EXPECT_GE(reservedBuffer.capacity(), reservedBuffer.size());

    // ------------------------------------------------
    // Display results
    // ------------------------------------------------
    std::cout << "\n========== FORMAT BENCHMARK ==========\n";

    std::cout << "Iterations: 10000\n";

    std::cout << "std::format(): "
              << formatTime << " us\n";

    std::cout << "std::format_to(): "
              << formatToTime << " us\n";

    std::cout << "std::format_to() + reserve(): "
              << reservedTime << " us\n";

    std::cout << "Reserved buffer capacity: "
              << reservedBuffer.capacity() << " bytes\n";

    std::cout << "======================================\n";
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