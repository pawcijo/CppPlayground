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

    void PrintName()
    {
        std::cout << "\033[1;35m" << mName << "\033[0m " << ": \n"
                  << std::endl;
    }

    virtual void PrintNotes()
    {
        PrintName();

        for (const auto &pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << BOLD_TEXT_END << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    virtual void ShowExample() = 0;
    virtual void ShowDemo() = 0;
};
