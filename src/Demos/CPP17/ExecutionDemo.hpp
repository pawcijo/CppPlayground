#pragma once

#include "Common/DemoBase.hpp"
#include <execution> // Include for execution

class ExecutionDemo : public DemoBase {
public:
    ExecutionDemo() {
        mName = "ExecutionDemo";
        mNotes = {{"Not implemented:", "Not implemented"}};
    }

    void PrintName() override {
        std::cout << "ExecutionDemo not implemented yet." << std::endl;
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
        std::cout << "ExecutionDemo example not implemented yet." << std::endl;
    }

    void ShowDemo() override {
        ShowExample();
    }
};