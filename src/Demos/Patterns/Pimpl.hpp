#pragma once

#include <Common/Common.hpp>
#include "Common/DemoBase.hpp"

#include <iostream>

class PimplExample : public DemoBase
{
public:
    PimplExample()
    {
        mName = "PimplExample";
        mNotes = {};
    };
    ~PimplExample() = default;

    void PrintNotes() override
    {

        std::cout << mName << ": \n"
                  << std::endl;

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
