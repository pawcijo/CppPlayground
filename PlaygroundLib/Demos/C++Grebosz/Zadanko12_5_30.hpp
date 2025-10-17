#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

namespace Z_12_5_30
{
void Zadanko();
}

class Zadanko12_5_30 : public DemoBase
{
public:
  Zadanko12_5_30()
  {
    mName = "Opis zadanka 12.5.30";
    mNotes = { { "Zadanko 12.5.30", R"(Opis na stronie : 416)" } };
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

    Z_12_5_30::Zadanko();
  }
  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }
};