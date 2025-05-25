#pragma once

#include "Common/DemoBase.hpp"

#include "Demos/Patterns/Adapter/ClientInterface.hpp"
#include "Demos/Patterns/Adapter/MyOldClass.hpp"
#include "Demos/Patterns/Adapter/OldClassAfapter.hpp"
#include <memory>

class AdapterDemo : public DemoBase
{
public:
  AdapterDemo()
  {
    mName = "AdapterDemo";
    mNotes = {
      { "Adapter:",
        R"(An adapter is a design pattern that allows incompatible interfaces to work together. It acts as a bridge between two incompatible interfaces, 
        enabling them to communicate and work together seamlessly.)" }
    };
  }

  void ShowExample(
    void (*printNotesCallback)(NoteFormat& notes) = nullptr) override
  {
    if (nullptr == printNotesCallback)
    {
      PrintNotes();
    }
    else
    {
      printNotesCallback(mNotes);
      ;
    }

    Example();
  }
  void ShowDemo() override
  {
    ShowExample();
  }

private:
  void Example()
  {
    std::unique_ptr<MyOldClass> oldClass = std::make_unique<MyOldClass>();
    std::unique_ptr<OldClassAdapter> adapter =
      std::make_unique<OldClassAdapter>(std::move(oldClass));

    ClientInterface& interface = *adapter;
    interface.doSomethingCool();
  }
};
