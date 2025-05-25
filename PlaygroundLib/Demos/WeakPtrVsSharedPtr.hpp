#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace WeakPtrVsShared
{
const std::vector<std::pair<std::string, std::string>> Notes = {
  { { "Purpose" },
    "A std::weak_ptr is a smart pointer that does not own the object it points "
    "to.\n"
    "Instead, it provides a way to observe or temporarily access an object "
    "that is\n"
    "managed by one or more std::shared_ptrs, without contributing to the "
    "reference count.\n" },
  { { "No Reference Count" },
    { "A std::weak_ptr does not affect the reference count of the shared_ptr "
      "it refers to. This \n"
      "helps avoid circular references (a situation where two or more "
      "shared_ptrs reference each other, \n"
      "preventing the reference count from ever reaching zero and causing "
      "memory leaks). \n" } },
  { { "Expiration" },
    { "Since a std::weak_ptr does not own the object, it can become expired "
      "when the last std::shared_ptr \n"
      "holding the object is destroyed. Before accessing the object, you must "
      "check if the weak_ptr \n"
      "is valid (i.e., the object still exists) by using .lock(), which "
      "returns \n"
      "a shared_ptr if the object is still alive. \n" } }
};

inline void PrintCustomNote()
{
  for (const auto& pair : Notes)
  {
    std::cout << BOLD_TEXT_START << pair.first << ": " << TEXT_FORMAT_CLEAR
              << pair.second << std::endl;
  }
}
}

class ExampleClass
{
  int variable;
  float another_variable;

public:
  ExampleClass()
    : variable(5)
    , another_variable(5.0001f) {};

  int Variable()
  {
    return variable;
  }
  float AnotherVariable()
  {
    return another_variable;
  }
};

inline void WeakPtrVsSharedPtrExample()
{
  LOG_START_FUNCTION();
  std::shared_ptr<ExampleClass> sPtr = std::make_shared<ExampleClass>();

  std::weak_ptr<ExampleClass> wPtr = sPtr;

  if (auto sPtr = wPtr.lock())
  { // lock returns a shared_ptr if sp1 is still valid
    std::cout << "Object is still alive" << std::endl;
  }
  else
  {
    std::cout << "Object has been destroyed" << std::endl;
  }

  sPtr.reset(); // sp1 is reset, reference count = 0, object is destroyed

  if (auto sp2 = wPtr.lock())
  {
    std::cout << "Object is still alive" << std::endl;
  }
  else
  {
    std::cout << "Object has been destroyed" << std::endl;
  }

  LOG_END_FUNCTION();
}

class WeakPtrVsSharedPtrDemo : public DemoBase
{
public:
  WeakPtrVsSharedPtrDemo()
  {
    mName = "WeakPtrVsSharedPtrDemo";
    mNotes = {};
  };
  ~WeakPtrVsSharedPtrDemo() = default;

  void PrintNotes() override
  {
    PrintName();
    WeakPtrVsShared::PrintCustomNote();
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
    }
    WeakPtrVsSharedPtrExample();
  }

  void ShowDemo(
    void (*printNotesCallback)(NoteFormat& notes) = nullptr) override
  {
    ShowExample(printNotesCallback);
  };
};
