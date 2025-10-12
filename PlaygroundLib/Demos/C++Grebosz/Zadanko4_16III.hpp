#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>
#include <string>

namespace Z4_16
{

inline void Zadanko()
{
  for (char znak = 'a'; znak <= 'h'; znak += 2)
  {
    std::cout << znak << static_cast<char>(znak + 1) << "\n";
  }

  int x = 0;
  std::cout << "Ile znakow w lini ? :\n";
  std::cin >> x;
  char znak = 'h' + 1;
  int counter = 0;
  while (znak < 'z')
  {
    for (int i = 0; i < x; ++i)
    {
      if (znak <= 'z')
      {
        std::cout << znak;
      }
      ++znak;
      counter++;
    }
    if (counter % x == 0)
    {
      std::cout << "\n";
    }
  }
}
}

class Zadanko4_16III : public DemoBase
{
public:
  Zadanko4_16III()
  {
    mName = "Opis zadanka 4.16 III";
    mNotes = { { "Zadanko 4.16 III", R"(Opis na stronie : 152)" } };
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

    Z4_16::Zadanko();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }
};
