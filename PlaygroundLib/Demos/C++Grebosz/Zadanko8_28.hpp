#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <map>
#include <print>
#include <string>
#include <tuple>
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
short unsigned BitCounter(unsigned short n);

constexpr std::vector<
  std::tuple<std::pair<unsigned short, std::string>, unsigned int>>
Combination()
{
  std::vector<std::tuple<std::pair<unsigned short, std::string>, unsigned int>>
    combinations;
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
    auto para = std::make_pair(i, combination);
    unsigned short count_of_bits = BitCounter(i);
    combinations.push_back(std::tie(para, count_of_bits));
  }
  return combinations;
}

void SortByUsedBits(
  std::vector<std::pair<unsigned short, size_t>>& combinations);

using CombinationType = unsigned short;
using Counted = size_t;
std::map<CombinationType, Counted> GetCountedCombiations(
  const std::vector<unsigned short>& values);

inline void Zadanko()
{
  auto random_values = RandomValuesGenerator(50, 16);

  // show combinations
  auto combinations = Combination();

  for (auto combination : combinations)
  {
    std::println("{} {} usedbits:{}",
                 std::get<0>(combination).first,
                 std::get<0>(combination).second,
                 std::get<1>(combination));
  }

  auto counted_combinations = GetCountedCombiations(random_values);
  std::vector<std::pair<unsigned short, size_t>> combinations_vec;
  for (const auto& [combination, count] : counted_combinations)
  {
    combinations_vec.push_back(std::make_pair(combination, count));
  }

  SortByUsedBits(combinations_vec);
  std::reverse(combinations_vec.begin(), combinations_vec.end());

  std::println("\nSorted by used bits:");
  unsigned short value = 5;
  for (const auto& [combination, count] : combinations_vec)
  {

    auto it = std::find_if(combinations.begin(),
                           combinations.end(),
                           [combination](const auto& pair)
                           { return std::get<0>(pair).first == combination; });

    if (value != std::get<1>(*it))
    {
      value = std::get<1>(*it);
      std::println("---- Number of used bits: {} ----", value);
    }

    if (it != combinations.end())
    {
      std::println("{}: {}", std::get<0>(*it).second, count);
    }
    else
    {
      std::println("{} - Unknown combination: {}", combination, count);
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
