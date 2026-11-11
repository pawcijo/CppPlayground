#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

class RangesDemo : public DemoBase
{
public:
  RangesDemo()
  {
    mName = "RangesDemo";
    mNotes = {
      { "<ranges>", "header provides range-based algorithms and views." },
      { "std::ranges::sort",
        "Sorts the elements in a range using a projection or comparator." },
      { "std::ranges::views::iota",
        "Creates a range of sequential values." },
      { "std::ranges::views::filter",
        "Keeps only elements satisfying a predicate." },
      { "std::ranges::views::transform",
        "Applies a transformation to each element." },
      { "std::ranges::views::take",
        "Restricts a range to the first N elements." },
      { "std::ranges::views::reverse",
        "Iterates over a range in reverse order." }
    };
  }

  ~RangesDemo() = default;

  void ShowExample(
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

    ViewIotaExample();
    FilterExample();
    TransformExample();
    SortExample();
    ReverseExample();

    LOG_END_FUNCTION_CLASS();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }

private:
  void ViewIotaExample()
  {
    LOG_START_FUNCTION();
    std::cout << "iota: ";
    for (int value : std::views::iota(1, 6))
    {
      std::cout << value << ' ';
    }
    std::cout << '\n';
    LOG_END_FUNCTION();
  }

  void FilterExample()
  {
    LOG_START_FUNCTION();
    std::cout << "even numbers: ";
    for (int value : std::views::iota(1, 11)
                       | std::views::filter([](int x) { return x % 2 == 0; }))
    {
      std::cout << value << ' ';
    }
    std::cout << '\n';
    LOG_END_FUNCTION();
  }

  void TransformExample()
  {
    LOG_START_FUNCTION();
    std::cout << "squared values: ";
    for (int value : std::views::iota(1, 6)
                       | std::views::transform([](int x) { return x * x; }))
    {
      std::cout << value << ' ';
    }
    std::cout << '\n';
    LOG_END_FUNCTION();
  }

  void SortExample()
  {
    LOG_START_FUNCTION();
    std::vector<int> values = { 9, 3, 5, 1, 7, 4 };
    std::ranges::sort(values);

    std::cout << "sorted: ";
    for (int value : values)
    {
      std::cout << value << ' ';
    }
    std::cout << '\n';
    LOG_END_FUNCTION();
  }

  void ReverseExample()
  {
    LOG_START_FUNCTION();
    std::vector<int> values = { 1, 2, 3, 4, 5 };
    std::cout << "reversed: ";
    for (int value : values | std::views::reverse)
    {
      std::cout << value << ' ';
    }
    std::cout << '\n';
    LOG_END_FUNCTION();
  }
};
