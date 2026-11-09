#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include "AtomicExamples/AtomicCounterExample.hpp"
#include "AtomicExamples/AtomicServerStatusExample.hpp"

class AtomicDemo : public DemoBase
{

public:
  auto RunDemo()
  {
    PlaygroundLib::AtomicExamples::CounterWithoutAtomic();
    PlaygroundLib::AtomicExamples::CounterWithAtomic();
    PlaygroundLib::AtomicExamples::ServerStatusWithoutAtomic();
    PlaygroundLib::AtomicExamples::ServerStatusWithAtomic();
  }

public:
  AtomicDemo()
  {
    mName = "AtomicDemo";
    mNotes = { { "std::atomic",
                 "Provides atomic operations for shared data." } };
  }

  ~AtomicDemo() override = default;

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

    RunDemo();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }
};
