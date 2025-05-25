#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include "Demos/Patterns/Singleton/Singleton.hpp"

class SingletonDemo : public DemoBase
{
public:
  SingletonDemo()
  {
    mName = "SingletonDemo";
    mNotes = {
      { "Singleton:",
        R"(
A singleton is a type instance for which there is one, and only one, object in the system.
Attributed to Scott Meyers, this singleton pattern exploits three important properties:)" },
      { "",
        R"(1.) Static function objects are initialized when control flow hits the function for the first time.)" },
      { "",
        R"(2.) The lifetime of function static variables begins the first time the program flow encounters
     the declaration and ends at program termination.)" },
      { "",
        R"(3.) If control enters the declaration concurrently while the variable is being initialized, 
    the concurrent execution shall wait for completion of the initialization)" }
    };
  };
  ~SingletonDemo() = default;

  void ShowExample(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
      if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);
        }
    Singleton::getInstance().doSomething();
  }

  void ShowDemo(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  };
};