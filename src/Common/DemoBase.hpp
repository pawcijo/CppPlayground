#pragma once

#include "Common.hpp"
#include <iostream>

class DemoBase
{
protected:
    std::string mName;
    NoteFormat mNotes;

public:

    DemoBase() = default;
    virtual ~DemoBase() {};

    virtual void PrintNotes()
    {
        for (const auto &pair : mNotes)
        {
            std::cout << mName << ": \n" << std::endl;

            std::cout << BOLD_TEXT_START << pair.first << BOLD_TEXT_END << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    virtual void ShowExample() = 0;
    virtual void ShowDemo() = 0;
};
