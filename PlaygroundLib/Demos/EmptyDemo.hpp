#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include "Demos/VirtualDestructor.hpp"

#include <memory>
#include <string>

namespace EmptyD
{
const NoteFormat notes = {
  { { "Virtual destructor and constructor behavior example. " }, { R"()" } }
};

inline void PrintCustomNotes()
{
  for (const auto& pair : notes)
  {
    std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
              << pair.second << std::endl
              << std::endl;
  }
}
}

class BaseClaseExample
{

public:
  BaseClaseExample()
  {
    std::cout << "BaseClaseExample Constructor \n";
  }

  virtual ~BaseClaseExample()
  {
    std::cout << "BaseClaseExample Destructor \n";
  }
};

class InheritedClassExample : public BaseClaseExample
{

public:
  InheritedClassExample()
  {
    std::cout << "InheritedClassExample Constructor \n";
  }

  ~InheritedClassExample() override
  {
    std::cout << "InheritedClassExample Destructor \n";
  }
};

class EmptyDemo : public DemoBase
{
public:
  EmptyDemo()
  {
    mName = "EmptyDemo";
    mNotes = {};
  };
  ~EmptyDemo() = default;

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
    }
    EmptyD::PrintCustomNotes();

    BaseClaseExample* example = new InheritedClassExample();

    delete example;
  }

  void ShowDemo(
    void (*printNotesCallback)(NoteFormat& notes) = nullptr) override
  {
    ShowExample(printNotesCallback);
  };
};
