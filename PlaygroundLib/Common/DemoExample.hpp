#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>
#include <utility>
#include <vector>

// Copy this code to create new example
class DemoExample : public DemoBase
{
public:
  DemoExample()
  {
    mName = "DemoExample";
    mNotes = { { "Paragraph:", "bla bla bla bla bla notes" } };
  };
  ~DemoExample() = default;

  void PrintNotes() override
  {
    PrintName();

    for (const auto& pair : mNotes)
    {
      std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR
                << std::endl
                << pair.second << std::endl
                << std::endl;
    }
  }

  void ShowExample(void (*printNotesCallback)() = nullptr) override
  {
      if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);;
        }
  }

  void ShowDemo() override
  {
    ShowExample();
  };
};
