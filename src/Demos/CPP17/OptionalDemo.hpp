#pragma once

#include "Common/DemoBase.hpp"
#include <optional> // Include for optional

class OptionalDemo : public DemoBase {
public:
    OptionalDemo() {
        mName = "OptionalDemo";
        mNotes = {{"Not implemented:", "Not implemented"}};
    }

    void PrintName() override {
        std::cout << "OptionalDemo not implemented yet." << std::endl;
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
        std::cout << "OptionalDemo example not implemented yet." << std::endl;
    }

    void ShowDemo() override {
        ShowExample();
    }
};