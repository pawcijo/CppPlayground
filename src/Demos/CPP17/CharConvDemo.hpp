#pragma once

#include "Common/DemoBase.hpp"
#include <charconv> // Include for charconv

class CharConvDemo : public DemoBase {
public:
    CharConvDemo() {
        mName = "CharConvDemo";
        mNotes = {{"Not implemented:", "Not implemented"}};
    }

    void PrintName() override {
        std::cout << "CharConvDemo not implemented yet." << std::endl;
    }

    void PrintNotes() override {
        PrintName();
        for (const auto &pair : mNotes) {
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample() override {
        std::cout << "CharConvDemo example not implemented yet." << std::endl;
    }

    void ShowDemo() override {
        ShowExample();
    }
};