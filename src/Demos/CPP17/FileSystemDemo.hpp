#pragma once

#include "Common/DemoBase.hpp"
#include <filesystem> // Include for filesystem

class FileSystemDemo : public DemoBase {
public:
    FileSystemDemo() {
        mName = "FileSystemDemo";
        mNotes = {{"Not implemented:", "Not implemented"}};
    }

    void PrintName() override {
        std::cout << "FileSystemDemo not implemented yet." << std::endl;
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
        std::cout << "FileSystemDemo example not implemented yet." << std::endl;
    }

    void ShowDemo() override {
        ShowExample();
    }
};