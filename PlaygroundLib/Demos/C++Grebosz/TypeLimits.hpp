#include "Common/DemoBase.hpp"
#include <cstdint>
#include <limits>
#include <type_traits>

#include <string_view>

template<typename T>
constexpr std::string_view type_name()
{
#ifdef __clang__
  std::string_view name = __PRETTY_FUNCTION__;
  auto start = name.find("T = ") + 4;
  auto end = name.find(']', start);
  return name.substr(start, end - start);
#elif defined(__GNUC__)
  std::string_view name = __PRETTY_FUNCTION__;
  auto start = name.find("with T = ") + 8;
  auto end = name.find(';', start);
  return name.substr(start, end - start);
#elif defined(_MSC_VER)
  std::string_view name = __FUNCSIG__;
  auto start = name.find("type_name<") + 10;
  auto end = name.find(">(void)");
  return name.substr(start, end - start);
#endif
}

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
// Apple Clang does not support std::print yet
#ifdef __APPLE__
      std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR
                << std::endl
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

    

    AppendLimitsInfo<short int>(notes);
    AppendLimitsInfo<int_least32_t>(notes);
    AppendLimitsInfo<int>(notes);
    AppendLimitsInfo<float>(notes);
    AppendLimitsInfo<double>(notes);
    AppendLimitsInfo<char>(notes);
    AppendLimitsInfo<bool>(notes);
    AppendLimitsInfo<wchar_t>(notes);
    AppendLimitsInfo<char16_t>(notes);
    AppendLimitsInfo<char32_t>(notes);
    notes.push_back({ "", "" });

    AppendLimitsInfo<int_fast16_t>(notes);
    AppendLimitsInfo<int_fast64_t>(notes);


    // Type sizes

    notes.push_back({ "", "" });

    printNotesCallback(notes);
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }

private:
  void AppendNote(const std::string& title,
                  const std::string& content,
                  NoteFormat& aNotes)
  {
    aNotes.push_back({ title, content });
  }

  template<typename T>
requires std::is_arithmetic_v<T>
void AppendLimitsInfo(NoteFormat& aNotes)
{
    std::string body;

    if constexpr (std::is_same_v<T, char> ||
                  std::is_same_v<T, signed char> ||
                  std::is_same_v<T, unsigned char>)
    {
        body = std::format("min = {} \nmax = {} \nsize = {} bytes\nSigned = {}",
                           static_cast<int>(std::numeric_limits<T>::min()),
                           static_cast<int>(std::numeric_limits<T>::max()),
                           sizeof(T),
                           std::numeric_limits<T>::is_signed ? "Yes" : "No");
    }
    else if constexpr (std::is_same_v<T, wchar_t>)
    {
        body = std::format("min = {} \nmax = {} \nsize = {} bytes\nSigned = {}",
                           static_cast<long long>(std::numeric_limits<T>::min()),
                           static_cast<long long>(std::numeric_limits<T>::max()),
                           sizeof(T),
                           std::numeric_limits<T>::is_signed ? "Yes" : "No");
    }
    else if constexpr (std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>)
    {
        body = std::format("min = {} \nmax = {} \nsize = {} bytes\nSigned = {}",
                           static_cast<unsigned long long>(std::numeric_limits<T>::min()),
                           static_cast<unsigned long long>(std::numeric_limits<T>::max()),
                           sizeof(T),
                           std::numeric_limits<T>::is_signed ? "Yes" : "No");
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        body = std::format("lowest = {} \nmax = {} \nsize = {} bytes\nSigned = {}",
                           std::numeric_limits<T>::lowest(),
                           std::numeric_limits<T>::max(),
                           sizeof(T),
                           std::numeric_limits<T>::is_signed ? "Yes" : "No");
    }
    else
    {
        body = std::format("min = {} \nmax = {} \nsize = {} bytes\nSigned = {}",
                           std::numeric_limits<T>::min(),
                           std::numeric_limits<T>::max(),
                           sizeof(T),
                           std::numeric_limits<T>::is_signed ? "Yes" : "No");
    }

    AppendNote(std::format("{} limits:", type_name<T>()), body, aNotes);
}

};
