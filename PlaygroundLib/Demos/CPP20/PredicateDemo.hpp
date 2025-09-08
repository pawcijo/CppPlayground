#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include <concepts>

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
namespace
{

using VarType = std::variant<int, std::pair<int, int>>;

template<typename T>
class Filter
{
public:
  Filter(std::vector<T> data)
    : data_(std::move(data))
  {
  }

  // Accepts a predicate constrained by std::predicate
  void apply(std::predicate<const int> auto pred)
  {
    std::vector<T> result;
    std::copy_if(data_.begin(), data_.end(), std::back_inserter(result), pred);

    // Print results
    std::cout << "Filtered results: ";
    for (const auto& item : result)
    {
      std::cout << item << " ";
    }
    std::cout << "\n";
  }

  void apply(std::predicate<const std::pair<int, int>&> auto pred)
  {
    std::vector<T> result;
    std::copy_if(data_.begin(), data_.end(), std::back_inserter(result), pred);

    // Print results
    std::cout << "Filtered results: ";
    for (const auto& item : result)
    {
      std::cout << "(" << item.first << ", " << item.second << ") ";
    }
    std::cout << "\n";
  }

private:
  std::vector<T> data_;
};

}

class PredicateDemo : public DemoBase
{
public:
  PredicateDemo()
  {
    mName = "PredicateDemo";
    mNotes = {
      { "std::preicate",
        "A type that represents a unary predicate, which is a function that "
        "takes one argument and returns a boolean value." },
      { "std::predicate",
        "A type that represents a predicate, which can be either unary or "
        "binary." },
      { "std::negation", "A type that negates the result of a predicate." },
      { "std::conjunction and std::disjunction",
        "Types that represent logical conjunction (AND) and disjunction (OR) "
        "of predicates." }

    };
  }

  ~PredicateDemo() = default;

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

    // Add examples of predicates here
    PredicateExample();
    TemplatePredicateExample();
    NegationExample();
    ConjunctionExample();
    DisjunctionExample();

    LOG_END_FUNCTION_CLASS();
  }

  void ShowExample(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowDemo(printNotesCallback);
  }

  void PredicateExample()
  {
    LOG_START_FUNCTION();
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };

    Filter<int> f(numbers);

    // Using a lambda predicate
    f.apply([](int x) { return x % 2 == 0; }); // filters even numbers

    f.apply([](int x) { return x > 3; }); // filters numbers greater than 3

    LOG_END_FUNCTION();
  }

  template<typename T, std::predicate<T> Pred>
  auto filter(const std::vector<T>& vec, Pred pred)
  {
    std::vector<T> result;
    for (const auto& elem : vec)
    {
      if (pred(elem))
      {
        result.push_back(elem);
      }
    }
    return result;
  }

  void TemplatePredicateExample()
  {

    struct ExampleType
    {
      int value;
      std::string name;
    };

    LOG_START_FUNCTION();
    std::vector<ExampleType> data = {
      { 1, "One" }, { 2, "Two" }, { 3, "Three" }, { 4, "Four" }
    };

    for (const auto& item : data)
    {
      std::cout << "Value: " << item.value << ", Name: " << item.name << "\n";
    }

    auto bigger_than_2 =
      filter(data, [](const ExampleType& item) { return item.value > 2; });

    std::cout << "Filtered results (bigger than 2):\n";
    for (const auto& element : bigger_than_2)
    {
      std::cout << "Value: " << element.value << ", Name: " << element.name
                << "\n";
    }

    LOG_END_FUNCTION();
  }

  template<typename T>
  std::enable_if_t<std::negation_v<std::is_integral<T>>, void> process(
    const T& value)
  {
    std::cout << "Processing non-integral: " << value << '\n';
  }
  //---------------------Negation Example---------------------

  template<typename T>
  void process_runtime(const T& value)
  {
    if constexpr (std::negation_v<std::is_integral<T>>)
    {
      std::cout << "Non-integral type at compile time: " << value << "\n";
    }
    else
    {
      std::cout << "Integral type: " << value << "\n";
    }
  }

  void NegationExample()
  {
    LOG_START_FUNCTION();

    std::vector<std::variant<int, double, std::string>> data = {
      42, 3.14, "Hello", 100, 2.718, "World"
    };

    for (const std::variant<int, double, std::string>& item : data)
    {
      std::visit([](const auto& value)
                 { std::cout << "Value: " << value << "\n"; },
                 item);
    }

    int intValue = 42;
    double doubleValue = 3.14;
    std::string stringValue = "Hello";
    process_runtime(intValue);
    process_runtime(doubleValue);
    process_runtime(stringValue);

    // process(intValue) ;//This will not compile, as int is an integral type
    process(
      doubleValue); // This will compile, as double is not an integral type
    process(
      stringValue); // This will compile, as string is not an integral type

    LOG_END_FUNCTION();
  }

  // End of Negation Example

  //---------------------Conjunction Example---------------------

  template<typename T>
  std::enable_if_t<std::conjunction_v<std::is_default_constructible<T>,
                                      std::is_copy_constructible<T>,
                                      std::negation<std::is_abstract<T>>>>
  print_info(const T&)
  {
    std::cout << "T is default & copy constructible, and not abstract\n";
  }

  struct GoodType
  {
    GoodType() = default;
    GoodType(const GoodType&) = default;
  };

  struct BadType
  {
    BadType(int)
    {
    } // Not default-constructible
    BadType(const BadType&) = default;
  };

  void ConjunctionExample()
  {
    LOG_START_FUNCTION();
    // Example of conjunction

    GoodType good;
    BadType bad(42);
    print_info(good); // This will compile
    // print_info(bad); // This will not compile, as BadType is not
    // default-constructible

    LOG_END_FUNCTION();
  }

  // End of Conjunction Example

  template<typename T>
  std::enable_if_t<
    std::disjunction_v<std::is_integral<T>, std::is_floating_point<T>>,
    void>
  print_number(T value)
  {
    std::cout << "Number: " << value << '\n';
  }

  // ---------------------Disjunction Example---------------------
  void DisjunctionExample()
  {
    LOG_START_FUNCTION();

    print_number(42);   // integral
    print_number(3.14); // floating point
    LOG_END_FUNCTION();
  }

  // End of Disjunction Example
};