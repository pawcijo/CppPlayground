#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include <cstddef>
#include <cstring>

class LongestCharSequence : public DemoBase
{
public:
  LongestCharSequence()
  {
    mName = "LongestCharSequence";
    mNotes = {
      { "LongestCharSequence:",
        R"(A task to find the longest sequence of identical characters in a string.)" }
    };
  }

  int FindLongestCharSequence(const char* input)
  {
    size_t string_length = std::strlen(input);
    if (string_length == 0)
      return 0;

    if (string_length == 1)
      return 1;

    int maxLength = 1;
    int currentLength = 1;

    // check previous character
    for (size_t next_character_index = 1; next_character_index < string_length;
         ++next_character_index)
    {
      if (input[next_character_index] == input[next_character_index - 1])
      {
        currentLength++;
        if (maxLength < currentLength)
        {
          maxLength = currentLength;
        }
      }
      else
      {
        currentLength = 1;
      }
    }

    return maxLength;
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

    std::cout << "Should be 6 is " << FindLongestCharSequence("abbbbcdaaaaaa")
              << "(abbbbcdaaaaaa)\n";
  }
};
