#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

class StringParser
{
public:
  static long long ParseString(const char* input)
  {
    long long value = 0;
    int sign = 1;
    bool skipFirst = false;

    // incorret string
    if (nullptr == input)
      return 0;

    // check sign
    if (input[0] == '-')
    {
      skipFirst = true;
      sign = -1;
    }

    if (input[0] == '+')
    {
      skipFirst = true;
      sign = 1;
    }

    // check if not only sign
    if (input[0] == '-' && input[1] == '\0')
    {
      return 0;
    }

    // Check if first character needs to be skipped
    const char* start_ptr = input;
    if (skipFirst)
    {
      ++start_ptr; // skip the '-'
    }

    // check if string is valied
    for (const char* character_ptr = start_ptr; *character_ptr != '\0';
         ++character_ptr)
    {
      char c = *character_ptr;
      if (c < '0' || '9' < c)
      {
        std::cout << "Wrong character, string not a number :[" << c << "]\n";
        return 0;
      }
    }

    // parse
    for (const char* character_ptr = start_ptr; *character_ptr != '\0';
         ++character_ptr)
    {
      value = value * 10 + (*character_ptr - '0');
    }

    return sign * value;
  }
};

class StringToIntoDemo : public DemoBase
{

public:
  StringToIntoDemo()
  {
    mName = "StringToInt";
    mNotes = {
      { "String to int conversion:",
        R"(A demonstration of converting strings without using std)" }
    };
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }

  void ShowExample(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    if (nullptr == printNotesCallback)
    {
      PrintNotes();
    }
    else
    {
      printNotesCallback(mNotes);
    }

    std::cout << "String to int 2137 ->" << StringParser::ParseString("2137")
              << "\n";
  }
};
