#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

namespace Z_13_9_10
{
void Zadanko();
}

class Zadanko13_9_10 : public DemoBase
{
public:
  Zadanko13_9_10()
  {
    mName = "Opis zadanka 12.9.10";
    mNotes = { { "Zadanko 12.9.10", R"(Opis na stronie : 445)" } };
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

    Z_13_9_10::Zadanko();
  }
  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }
};