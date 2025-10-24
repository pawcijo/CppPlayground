#include "Zadanko13_9_10.hpp"
#include <format>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace Z_13_9_10
{
const auto tabSize = 10;
auto Implementacja()
{

  char** tab = new char*[10];
  char** linijka = tab;

  for (int i = 0; i < tabSize; ++i)
  {
    char* charTab = new char[80];
    linijka[i] = charTab;
    strcpy(linijka[i], "To jest linijka nr n");
    linijka[i][19] = '0' + i;
  }

  return linijka;
}

// how it sould be done
auto BetterImpl(std::size_t howMany)
{
  std::vector<std::string> strings;
  strings.reserve(howMany);
  for (size_t i = 0; i < howMany; ++i)
  {
    strings.emplace_back(std::format("To jest linijka nr {}", i));
  }
  return strings;
}

template<typename T>
void showValues(T container, unsigned long maxSize)
{
  //
  std::cout << "This is template function:(" << __FUNCTION__ << ")\n";
  for (size_t i = 0; i < maxSize; ++i)
  {
    std::cout << container[i] << "\n";
  }
  std::cout << "Template function end.(" << __FUNCTION__ << ")\n";
}

void Zadanko()
{

  auto value = Implementacja();
  showValues(value, 10);
  std::cout << "\n";

  auto tmp = value[6];
  value[6] = value[3];
  value[3] = tmp;

  showValues(value, 10);

  for (int i = 0; i < tabSize; ++i)
  {
    delete[] value[i];
  }
  delete[] value;

  // with modern c++
  auto newValue = BetterImpl(tabSize);
  showValues(newValue, 10);
  std::cout << "\n";
  std::swap(newValue[3],newValue[6]); //swap is just tmp copy but without copy constructors [move semantics]

  showValues(newValue, 10);
  std::cout << "\n";
}
}
