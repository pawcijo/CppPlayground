#pragma once

#include <Common/Common.hpp>
#include "Common/DemoBase.hpp"

#include "Pimpl/LargeClass.hpp"

#include <iostream>
#include <memory>

class PimplExample : public DemoBase
{
public:
    PimplExample()
    {
        mName = "PimplExample";
        mNotes = {{{"Why use pimpl ?:"},{
R"(1.Decreases build times by reducing compilation dependencies
between class clients and class implementations.).
2.Improved Encapsulation (Hiding Implementation Details).
3.Reduced Compilation Dependencies(Less headers and includes).
4.Binary Compatibility : Pimpl enables binary compatibility across different versions of a library.
5.Stable ABI (Application Binary Interface).)"}},
        {{"Why NOT to use pimpl ?"},{
R"(1.Performance Overhead(std::unique_ptr and Memory locality is disrupted)
2.Code complexity (boilerplate code, more complex interface-implementation)
3.Increased Dynamic Memory (std::unique_ptr or std::shared_ptr uses new and delete inside)
4.Thread-Safety Concerns (mutex my be needed to synchronize)
5.No Inlining))"}}};
    };
    ~PimplExample() = default;

    void PrintNotes() override
    {

        PrintName();

        for (const auto &pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample(void(*printNotesCallback)(NoteFormat& notes) = nullptr) override
    {
          if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);;
        }

        LargeClass largeClass;
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
