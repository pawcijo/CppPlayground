#include "Common/DemoBase.hpp"
#include <limits>
#include <print>

class TypeLimitsDemo : public DemoBase
{
public:
  TypeLimitsDemo()
  {
    mName = "TypeLimits";
    mNotes = {
      { "TypeLimits:",
        R"(A demonstration of type limits in C++ using the <limits> header.)" }
    };
  }

  void PrintNotes() override
  {
    PrintName();
    for (const auto& pair : mNotes)
    {
      //Apple Clang does not support std::print yet
      #ifdef __APPLE__
      std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                << pair.second << std::endl
                << std::endl;
      #else
      std::println(
        BOLD_TEXT_START "{}" TEXT_FORMAT_CLEAR "{}", pair.first, pair.second);
      #endif
    }
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

    NoteFormat notes;

    notes.push_back({ "Type Limits Demonstration", "" });

    notes.push_back({ "int limits:",
                      std::format("min = {} \nmax = {} ",
                                  std::numeric_limits<int>::min(),
                                  std::numeric_limits<int>::max()) });

    notes.push_back({ "double limits:",
                      std::format("min = {}\nmax = {}",
                                  std::numeric_limits<double>::min(),
                                  std::numeric_limits<double>::max()) });
    notes.push_back({ "char limits:",
                      std::format("min = {}\nmax = {}",
                                  (int)std::numeric_limits<char>::min(),
                                  (int)std::numeric_limits<char>::max()) });
    notes.push_back({ "bool limits",
                      std::format("min = {}\nmax = {}",
                                  std::numeric_limits<bool>::min(),
                                  std::numeric_limits<bool>::max()) });
    notes.push_back({ "", "" });

    notes.push_back(
      { "Is int signed?",
        std::format("{}",
                    std::numeric_limits<int>::is_signed ? "Yes" : "No") });
    notes.push_back(
      { "Is float signed?",
        std::format("{}",
                    std::numeric_limits<float>::is_signed ? "Yes" : "No") });
    notes.push_back(
      { "Is char signed? ",
        std::format("{}",
                    std::numeric_limits<char>::is_signed ? "Yes" : "No") });
    notes.push_back(
      { "Is bool signed?",
        std::format("{}",
                    std::numeric_limits<bool>::is_signed ? "Yes" : "No") });
    notes.push_back({ "", "" });

    // Type sizes
    notes.push_back({ "Type sizes is bytes:", "" });
    notes.push_back({ "int:", std::format("{}", sizeof(int)) });
    notes.push_back({ "float:", std::format("{}", sizeof(float)) });
    notes.push_back({ "double:", std::format("{}", sizeof(double)) });
    notes.push_back({ "char:", std::format("{}", sizeof(char)) });
    notes.push_back({ "bool:", std::format("{}", sizeof(bool)) });
    notes.push_back({ "wchar_t:", std::format("{}", sizeof(wchar_t)) });
    notes.push_back({ "char16_t:", std::format("{}", sizeof(char16_t)) });
    notes.push_back({ "char32_t:", std::format("{}", sizeof(char32_t)) });
    notes.push_back({ "", "" });

    printNotesCallback(notes);
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }
};
