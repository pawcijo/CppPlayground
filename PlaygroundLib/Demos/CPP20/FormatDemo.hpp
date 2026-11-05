#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <format>

namespace
{
static std::size_t allocations = 0;
};

class FormatDemo : public DemoBase
{

public:
  FormatDemo()
  {
    mName = "FormatDemo";
    mNotes = {
      {
        { "Format specifications:" },
        { "The text formatting library offers a safe and extensible "
          "alternative to the printf family of functions."
          " It is intended to complement the existing C++ I/O streams "
          "library." },
      },
      { { "format:" },
        { "Stores formatted representation of the arguments in a new "
          "string." } },
      { { "format_to:" },
        { "Writes out formatted representation of its arguments through an "
          "output iterator." } },
      { { "format_to_n:" },
        { "Writes out formatted representation of its arguments through an "
          "output iterator, not exceeding specified size." } },
      { { "formatted_size:" },
        { "Determines the number of characters necessary to store the "
          "formatted representation of its arguments." } },
    };
  }

  ~FormatDemo() = default;

  auto Example()
  {
    LOG_START_FUNCTION_CLASS();
    std::string message = std::format("The answer is {} \n.", 42);
    std::cout << "Fromatted message: " << message;
    LOG_END_FUNCTION_CLASS();
  }

  auto FormatToExamle()
  {
    LOG_START_FUNCTION_CLASS();
    std::string buffer;

    std::format_to(std::back_inserter(buffer), // < OutputIt
                   "Hello, C++{}!\n",          // < fmt
                   "20"                        // < arg
    );
    std::cout << buffer;
    LOG_END_FUNCTION_CLASS();
  }
  auto FormatedSize()
  {
    LOG_START_FUNCTION_CLASS();
    int userId = 42;
    std::string name = "John";

    const auto size =
      std::formatted_size("User ID: {}, Name: {}", userId, name);

    std::cout << "Required size: " << size << '\n';
    LOG_END_FUNCTION_CLASS();
  }

  auto FormatToN()
  {
    LOG_START_FUNCTION_CLASS();
    int userId = 42;
    std::string name = "John";

    std::string buffer(12, '\0');

    auto result = std::format_to_n(
      buffer.begin(), buffer.size(), "User ID: {}, Name: {}", userId, name);

    buffer.resize(result.size);

    std::cout << "Formatted: " << buffer << '\n';
    std::cout << "Required size: " << result.size << '\n';
    LOG_END_FUNCTION_CLASS();
  }

  auto FormatToN_WithSize()
  {
    LOG_START_FUNCTION_CLASS();
    int userId = 42;
    std::string name = "John";

    constexpr auto formatString = "User ID: {}, Name: {}";

    // Calculate required size
    const auto requiredSize = std::formatted_size(formatString, userId, name);

    // Allocate the required buffer
    std::string buffer(requiredSize, '\0');

    // Format directly into the buffer
    auto result = std::format_to_n(
      buffer.begin(), buffer.size(), formatString, userId, name);

    std::cout << buffer << '\n';
    std::cout << "Characters written: " << (result.out - buffer.begin())
              << '\n';
    LOG_END_FUNCTION_CLASS();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    LOG_START_FUNCTION_CLASS();
    if (nullptr == printNotesCallback)
    {
      PrintNotes();
    }
    else
    {
      printNotesCallback(mNotes);
    }

    Example();
    FormatToExamle();
    FormatedSize();
    FormatToN();
    FormatToN_WithSize();

    LOG_END_FUNCTION_CLASS();
  }

  void ShowExample(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowDemo(printNotesCallback);
  }
};