#pragma once

#include "Common/DemoBase.hpp"
#include <memory_resource> // Include for memory_resource

class MemoryResourceDemo : public DemoBase {
public:
    MemoryResourceDemo() {
        mName = "MemoryResourceDemo";
        mNotes = {{"Not implemented:", "Not implemented"}};
    }

    void PrintName() override {
        std::cout << "MemoryResourceDemo not implemented yet." << std::endl;
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
        std::cout << "MemoryResourceDemo example not implemented yet." << std::endl;
    }

    void ShowDemo() override {
        ShowExample();
    }
};