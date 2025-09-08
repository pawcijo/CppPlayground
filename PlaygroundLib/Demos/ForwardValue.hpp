#pragma once

#include <memory.h>
#include <string.h>

#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

class ForwardDemo : public DemoBase
{
  std::unique_ptr<std::string> helper_variable;
  std::string helper_variable_2;
  std::string helper_variable_3;

public:
  ForwardDemo()
    : helper_variable(std::make_unique<std::string>("Help me 1"))
    , helper_variable_2("Help me 2")
    , helper_variable_3("Help me 3")
  {
    mName = "ForwardDemo";
    mNotes = {
      { { "Fast note:" },
        {
          R"("In short, rvalue references should be unconditionally cast to rvalues (via std::move)
when forwarding them to other functions, because they’re always bound to rvalues,
and universal references should be conditionally cast to rvalues (via std::forward)
when forwarding them, because they’re only sometimes bound to rvalues." -  Effective modern c++ 169)" } }
    };
  }
  void MoveEmaple(std::unique_ptr<std::string> str_ptr)
  {
    LOG_START_FUNCTION();
    std::cout << "\033[1;3m" << "MoveEmaple" << TEXT_COLOR_CLEAR << "\n";
    helper_variable = std::move(str_ptr);
    LOG_END_FUNCTION();
  }

  template<typename T>
  void ForwardEmaple(T&& t_value)
  {
    LOG_START_FUNCTION();
    helper_variable_2 = std::forward<T>(t_value);
    LOG_END_FUNCTION();
  }

  template<typename T>
  void DontDoThat(T&& t_value)
  {
    LOG_START_FUNCTION();
    // Dont move it !
    // t_value might be a universal reference not a rvalue.
    helper_variable_3 = std::move(t_value);
    LOG_END_FUNCTION();
  }

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

    MoveEmaple(std::make_unique<std::string>("Test"));

    auto someString = std::string("Test2");
    ForwardEmaple(someString);

    std::cout << "SomeString value before " << BOLD_TEXT_START << "DontDoThat()"
              << TEXT_FORMAT_CLEAR << " call:" << someString << std::endl;
    // If you this
    DontDoThat(someString);

    // helper_variable_3 will take someString value
    // and someString  will have unspecified value.
    //

    std::cout << "SomeString value after " << BOLD_TEXT_START << "DontDoThat()"
              << TEXT_FORMAT_CLEAR << " call: " << someString << std::endl;
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }
};
