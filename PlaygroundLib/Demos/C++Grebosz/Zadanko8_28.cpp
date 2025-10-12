#include "Zadanko8_28.hpp"
#include <cstdlib>
#include <ctime>

namespace Z_8_28
{

// generate n random values with max value
auto RandomValuesGenerator(int size, short max_value)
  -> std::vector<unsigned short>
{
    srand(time(0));
  std::vector<unsigned short> values;
  for (int i = 0; i < size; ++i)
  {
    auto value = rand() % max_value;
    // std::println("value: {}", value);
    values.push_back(value);
  }
  return values;
}

auto GetCountedCombiations(const std::vector<unsigned short>& values)
  -> std::map<CombinationType, Counted>
{
  std::map<CombinationType, Counted> counted_combinations;
  for (const auto& value : values)
  {
    counted_combinations[value]=counted_combinations[value]+1;
  }
  return counted_combinations;
}

}