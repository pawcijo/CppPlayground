#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>
#include <vector>
#include <utility>

//This string_view
#include <string_view>

//TODO
class StringViewDemo : public DemoBase
{
public:
    StringViewDemo()
    {
        mName = "StringViewDemo";
        mNotes = {
            {"Paragraph:", "bla bla bla bla bla notes"}};
    };
    ~StringViewDemo() = default;

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

    void ShowExample() override
    {
        PrintNotes();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
