#pragma once

#include "Common/Common.hpp"

#include <array>
#include <iostream>
#include <string_view>
#include <type_traits>

#include "Common/DemoBase.hpp"

constexpr const char* expr = "First expression";
constexpr std::string_view kekw = "string_view string";

constexpr std::string MergeStrings(const char* first, const char* second)
{
  // this is not a constant expression as
  // std::string() will run at runtime
  return std::string(first) + std::string(second);
}

constexpr std::string MergeStringViews(std::string_view first,
                                       std::string_view second)
{

  if (!std::is_constant_evaluated())
  {
    std::cout << "Runtime evaluated.\n";
  }
  else
  {
    std::cout << "Const evaluated.\n";
  }
  // this is not a constant expression as
  // std::string() will run at runtime
  return std::string(first) + std::string(second);
}

// Proper constant expression
constexpr int AddInCompileTime(auto first, auto second)
{
  if (!std::is_constant_evaluated())
  {
    std::cout << "Runtime addition\n"; // runtime only
  }
  return first + second;
}

class Point
{

  std::array<double, 2> values; // x and y

public:
  constexpr Point(double a_X, double a_Y)
    : values()
  {
    values[0] = a_X;
    values[1] = a_Y;
  }
  constexpr Point(std::initializer_list<double> aList)
    : values()
  {
    int count = 0;
    for (double value : aList)
    {
      values[count] = value;
      count++;
    }
  }
  constexpr Point(Point& another)
    : values()
  {
    values[0] = another.values[0];
    values[1] = another.values[1];
  }

  constexpr Point& operator=(const Point& another)
  {
    values[0] = another.values[0];
    values[1] = another.values[1];
    return *this;
  }

  constexpr Point& operator=(std::initializer_list<double> aList)
  {
    int count = 0;
    for (double value : aList)
    {
      values[count] = value;
      count++;
    }
    return *this;
  }

  Point() = delete;
};

constexpr auto value = 5;
constexpr auto value_2 = 5;

constexpr auto lista = { 2.0, 1.0 };
constexpr Point point = lista;

inline void ConstExpressionExample()
{
  LOG_START_FUNCTION();

  constexpr auto arraySize2 = 10;
  std::array<int, arraySize2> data2;

  std::string xd = expr;

  std::string expr_2 = " second string";

  std::cout << MergeStrings(expr, expr_2.c_str()) << std::endl;

  std::cout << MergeStringViews(kekw, kekw) << std::endl;

  int a_int = 10;
  int b_int = 5;

  auto runtime_value = AddInCompileTime(a_int, b_int); // runtime call
  std::cout << "Runtime value: " << runtime_value << "\n";

  // Compile-time arguments
  constexpr int const_value = AddInCompileTime(3, 4); // compile-time call
  std::cout << "Compile-time value: " << const_value << "\n";

  std::cout << "Array size: " << data2.size() << std::endl;

  LOG_END_FUNCTION();
}

class ConstExpressionDemo : public DemoBase
{
public:
  ConstExpressionDemo()
  {
    mName = "ConstExpressionDemo";
    mNotes = {};
  };
  ~ConstExpressionDemo() = default;

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
    ConstExpressionExample();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  };
};
