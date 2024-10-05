#pragma once

#include <memory.h>
#include <string.h>

#include <memory>
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
        : helper_variable(std::make_unique<std::string>("Help me 1")),
          helper_variable_2("Help me 2"),
          helper_variable_3("Help me 3")
    {
        mName = "ForwardDemo";
        mNotes = {{{"Fast note:"},
        {
R"(when forwarding them to other functions, because they’re always bound to rvalues,
and universal references should be conditionally cast to rvalues (via std::forward)
when forwarding them, because they’re only sometimes bound to rvalues.)"}}};
    }
    void MoveEmaple(std::unique_ptr<std::string> str_ptr)
    {
        helper_variable = std::move(str_ptr);
    }

    template <typename T>
    void ForwardEmaple(T&& t_value)
    {
        helper_variable_2 = std::forward(t_value);
    }

    template <typename T>
    void DontDoThat(T&& t_value)
    {
        helper_variable_3 = std::move(t_value);
    }

    void ShowExample() override
    {
        PrintNotes();
    }
    void ShowDemo() override
    {
        ShowExample();
    }
};