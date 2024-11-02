#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>
#include <vector>
#include <utility>

//This variant
#include <variant>

//TODO
class VariantDemo : public DemoBase
{
public:
    VariantDemo()
    {
        mName = "VariantDemo";
        mNotes = {
            {"Paragraph:", "bla bla bla bla bla notes"}};
    };
    ~VariantDemo() = default;

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
