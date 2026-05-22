#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

class DecodeStringDemo : public DemoBase
{
public:
  DecodeStringDemo()
  {
    mName = "DecodeStringDemo";
    mNotes = { { "DecodeStringDemo:", R"(TODO)" } };
  }

  std::string decompress(const std::string& compressedStr, bool debug = false)
  {
    std::string decompressed;

    for (int i = 0; i < static_cast<int>(compressedStr.size()); i++)
    {
      if (compressedStr[i] >= 'a' && compressedStr[i] <= 'z')
      {
        decompressed += compressedStr[i];
        continue;
      }
      if (compressedStr[i] >= '0' && compressedStr[i] <= '9')
      {

        std::string numberString;
        numberString += compressedStr[i];
        while (compressedStr[i + 1] >= '0' && compressedStr[i + 1] <= '9')
        {
          numberString += compressedStr[i + 1];
          i++;
        }

        // set to first character  after '['
        i = i + 2;
        std::string currentSubStr{};
        while (compressedStr[i] >= 'a' && compressedStr[i] <= 'z')
        {

          currentSubStr += compressedStr[i];
          i++;
        }

        int number = std::stoi(numberString);
        for (int k = 0; k < number; k++)
        {
          if (debug)
            decompressed += (" " + currentSubStr);
          else
          {
            decompressed += currentSubStr;
          };
        }
      }
    }

    return decompressed;
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

    std::string test = "favb3[ab]5[dds]";
    std::cout << test << std::endl;
    std::string decompresses = decompress(test, true);
    std::cout << decompresses << std::endl;

    std::string test2 = "favb0[ab]5[dds]";
    std::cout << test2 << std::endl;
    std::string decompresses2 = decompress(test2, true);
    std::cout << decompresses2 << std::endl;

    std::string test3 = "favb0[ab]0[dds]";
    std::cout << test3 << std::endl;
    std::string decompresses3 = decompress(test3, true);
    std::cout << decompresses3 << std::endl;

        std::string test4 = "0[ab]0[dds]favb";
    std::cout << test4 << std::endl;
    std::string decompresses4 = decompress(test4, true);
    std::cout << decompresses4 << std::endl;
  }
};
