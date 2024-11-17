#pragma once

#include <iostream>

#include "Common.hpp"

class DemoBase
{
   protected:
    std::string mName;
    NoteFormat mNotes;

   public:
    DemoBase() = default;
    virtual ~DemoBase(){};

    /*
    Print name of the demo.
    */
    virtual void PrintName()
    {
        std::cout << "\033[1;35m" << mName << "\033[0m "
                  << ": \n"
                  << std::endl;
    }

    virtual void PrintNotes()
    {
        PrintName();

        for (const auto &pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    virtual void ShowExample() = 0;

    virtual void ShowDemo()
    {
        ShowExample();
    };
};
