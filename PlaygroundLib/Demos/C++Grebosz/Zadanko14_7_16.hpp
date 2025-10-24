#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

class Zadanko14_7_16 : public DemoBase
{

public:
  Zadanko14_7_16()
  {
    mName = "Opis zadanka 14.7.16";
    mNotes = { { "Zadanko 14.7.16", R"(Opis na stronie : 469)" } };
  }

  void Zadanko();

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
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);

    Zadanko();
  }
};