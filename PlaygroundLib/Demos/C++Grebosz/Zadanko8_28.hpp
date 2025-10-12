#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <cstddef>
#include <map>
#include <print>
#include <string>
#include <utility>
#include <vector>

namespace Z_8_28
{

// FRS 1 0 0 0
// GER 0 1 0 0
// HEC 0 0 1 0
// DGF 0 0 0 1

constexpr short FRS = 0b1000;
constexpr short GER = 0b0100;
constexpr short HEC = 0b0010;
constexpr short DGF = 0b0001;

// generate n random values with max value
std::vector<unsigned short> RandomValuesGenerator(int size, short max_value);
constexpr std::vector<std::pair<unsigned short, std::string>> Combination()
{
  std::vector<std::pair<unsigned short, std::string>> combinations;
  for (auto i = 0; i < 16; ++i)
  {
    std::string combination;
    {
      if (i & FRS)
      {
        combination += "FRS ";
      }
      if (i & GER)
      {
        combination += "GER ";
      }
      if (i & HEC)
      {
        combination += "HEC ";
      }
      if (i & DGF)
      {
        combination += "DGF ";
      }
    }
    combinations.push_back(std::make_pair(i, combination));
  }
  return combinations;
}

using CombinationType = unsigned short;
using Counted = size_t;
 std::map<CombinationType, Counted> GetCountedCombiations(
  const std::vector<unsigned short>& values);

inline void Zadanko()
{
  auto random_values = RandomValuesGenerator(1000, 16);

  // show combinations
  auto combinations = Combination();

  for (auto combination : combinations)
  {
    std::println("{} {}", combination.first, combination.second);
  }

    auto counted_combinations = GetCountedCombiations(random_values);
  
  for(const auto& [combination, count] : counted_combinations)
  {
    auto it = std::find_if(
      combinations.begin(),
      combinations.end(),
      [combination](const auto& pair)
      {
        return pair.first == combination;
      });
    if (it != combinations.end())
    {
      std::println("Combination: {:<15} Count: {}", it->second, count);
    }
    else
    {
      std::println("Combination: {:<15} Count: {}", "Unknown", count);
    }
    
  }
}
}

class Zadanko8_28 : public DemoBase
{
public:
  Zadanko8_28()
  {
    mName = "Opis zadanka 8.28";
    mNotes = { { "Zadanko 8.28", R"(Opis na stronie : 309)" } };
  };

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

    Z_8_28::Zadanko();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }
};
