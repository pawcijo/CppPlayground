#pragma once
#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <compare>

class CompareDemo : public DemoBase
{

  struct Person
  {
    std::string name;
    int age;

    std::weak_ordering operator<=>(const Person& other) const
    {
      return age <=> other.age;
    }
  };

public:
  CompareDemo()
  {
    mName = "CompareDemo";
    mNotes = {
      { "<compare>", "header provides comparison functions and types." },
      { "std::strong_ordering",
        "A type that represents a strong ordering between two values." },
      { "std::weak_ordering",
        "A type that represents a weak ordering between two values." },
      { "std::partial_ordering",
        "A type that represents a partial ordering between two values." },
      { "std::compare_three_way",
        "A function that performs a three-way comparison between two "
        "values." }
    };
  }

  ~CompareDemo() = default;

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

    StrongOrderingExample();
    WeakOrderingExample();
    PartialOrderingExample();
    LOG_END_FUNCTION_CLASS();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }

  void StrongOrderingExample()
  {
    LOG_START_FUNCTION();
    int a = 3, b = 5;

    // Strong ordering: built-in types use this
    auto res1 = a <=> b;

    // Always returns eual for std::strong_ordering in this case
    // auto res2 = b <=> b;

    auto res3 = b <=> a;

    if (res1 == std::strong_ordering::less)
      std::cout << "3 is less than 5\n";

    // if (res2 == std::strong_ordering::equal)
    //   std::cout << "5 is equal to 5\n";

    if (res3 == std::strong_ordering::greater)
      std::cout << "5 is greater than 3\n";

    LOG_END_FUNCTION();
  }

  void WeakOrderingExample()
  {
    LOG_START_FUNCTION_CLASS();
    Person alice{ "Alice", 30 };
    Person bob{ "Bob", 40 };
    Person charlie{ "Charlie", 30 };

    if ((alice <=> bob) == std::weak_ordering::less)
      std::cout << "Alice is younger than Bob\n";

    if ((alice <=> charlie) == std::weak_ordering::equivalent)
      std::cout << "Alice and Charlie are the same age\n";

    if ((bob <=> alice) == std::weak_ordering::greater)
      std::cout << "Bob is older than Alice\n";

    LOG_END_FUNCTION_CLASS();
  }

  void PartialOrderingExample()
  {

    LOG_START_FUNCTION();

    // Example of partial ordering
    double x = 1.0;
    double y = std::nan(""); // Not a number
    double z = 2.0;
    auto res1 = x <=> y; // x is comparable to y
    auto res2 = y <=> z; // y is not comparable to z
    auto res3 = z <=> x; // z is comparable to x

    if (res1 == std::partial_ordering::unordered)
      std::cout << "x is unordered with y (NaN)\n";

    if (res2 == std::partial_ordering::unordered)
      std::cout << "y is unordered with z (NaN)\n";

    if (res3 == std::partial_ordering::greater)
      std::cout << "z is greater than x\n";

    LOG_END_FUNCTION();
  }
};
