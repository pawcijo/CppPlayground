#pragma once

#include "Common/Common.hpp"

#include <iostream>
#include <vector>
#include <utility>

class StaticMemoryAllocationDemo : public DemoBase
{
public:
    StaticMemoryAllocationDemo()
    {
        mName = "StaticMemoryAllocationDemo";
        mNotes = {
            {"Global Variables", "Variables declared outside of any function or class, accessible from any part of the program."},
            {"Static Variables", "Variables declared with the static keyword inside a function or class. "
                                 "These variables retain their value between function calls and are not reinitialized each time the function is called."},
            {"Static Members of Classes", "In object-oriented languages like C++, static class members (both data and functions) are stored in the static memory area. "
                                          "These members are shared across all instances of the class."},
            {"String Literals and Constant Data", "In some programming languages, string literals and other constant data are also stored in the static memory area."}};
    };
    ~StaticMemoryAllocationDemo() = default;

    void PrintNotes() override
    {
        PrintName();

        for (const auto &pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << BOLD_TEXT_END << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample() override
    {
        PrintNotes();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
