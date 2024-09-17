#pragma once

#include "Common/Common.hpp"

#include <iostream>
#include <vector>
#include <utility>

namespace StaticMemoryAllocation
{

    std::vector<std::pair<std::string, std::string>> Notes =
        {
            {"Global Variables", "Variables declared outside of any function or class, accessible from any part of the program."},
            {"Static Variables", "Variables declared with the static keyword inside a function or class. "
                                 "These variables retain their value between function calls and are not reinitialized each time the function is called."},
            {"Static Members of Classes", "In object-oriented languages like C++, static class members (both data and functions) are stored in the static memory area. "
                                          "These members are shared across all instances of the class."},
            {"String Literals and Constant Data", "In some programming languages, string literals and other constant data are also stored in the static memory area."}};

    void PrintNote()
    {
        for (const auto &pair : Notes)
        {
            std::cout << BOLD_TEXT_START << pair.first << ": " << BOLD_TEXT_END << pair.second << std::endl;
        }
    }

    void StaticMemoryAllocation()
    {
        LOG_START_FUNCTION();

        LOG_END_FUNCTION();
    }

    void StaticMemoryAllocationDemo()
    {
        PrintNote();
        StaticMemoryAllocation();
    }
}