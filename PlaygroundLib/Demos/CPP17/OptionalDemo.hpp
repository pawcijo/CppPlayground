#pragma once

#include "Common/DemoBase.hpp"
#include <optional> // Include for optional

class OptionalDemo : public DemoBase {
public:
    OptionalDemo() {
        mName = "OptionalDemo";
        mNotes = {{"std::optional", 
R"(std::optional is a type template in C++ that represents an optional value—it either contains 
a value or it does not (i.e., it is empty). This is useful in scenarios where a function might return a value but 
also may fail or not produce any meaningful result. Instead of using special sentinel values (like nullptr, -1, etc.), 
std::optional clearly expresses the absence of a value)"}};
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
       
        PrintNotes();
        BasicExample();
    }

    void ShowDemo() override {
        ShowExample();
    }

    private:

    void BasicExample()
    {
        LOG_FUNCTION_NAME(YELLOW);

        // Create an optional int
        std::optional<int> optInt;

        // Check if the optional has a value
        if (optInt.has_value())
        {
            std::cout << "Optional has a value: " << optInt.value() << std::endl;
        }
        else
        {
            std::cout << "Optional is empty" << std::endl;
        }

        // Assign a value to the optional
        optInt = 42;

        // Check if the optional has a value
        if (optInt.has_value())
        {
            std::cout << "Optional has a value: " << optInt.value() << std::endl;
        }
        else
        {
            std::cout << "Optional is empty" << std::endl;
        }

        // Reset the optional
        optInt.reset();

        // Check if the optional has a value
        if (optInt.has_value())
        {
            std::cout << "Optional has a value: " << optInt.value() << std::endl;
        }
        else
        {
            std::cout << "Optional is empty" << std::endl;
        }
    }
};