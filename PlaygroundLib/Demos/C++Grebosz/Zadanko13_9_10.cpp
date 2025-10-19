#include "Zadanko13_9_10.hpp"
#include <cstdlib>
#include <cstring>

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

void Zadanko()
{

  auto value = Implementacja();
  for (int i = 0; i < tabSize; ++i)
  {
    auto str = value[i];
    std::cout << str << "\n";
  }

  std::cout << "\n";

  auto tmp = value[6];
  value[6] = value[3];
  value[3] = tmp;

  for (int i = 0; i < tabSize; ++i)
  {
    auto str = value[i];
    std::cout << str << "\n";
  }

  for (int i = 0; i < tabSize; ++i)
  {
    delete[] value[i];
  }
  delete[] value;
}
}
