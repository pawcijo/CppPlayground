#pragma once

#include <charconv>  // Include for charconv

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

class CharConvDemo : public DemoBase
{
   public:
    CharConvDemo()
    {
        mName = "CharConvDemo";
        mNotes = {
            {"<charconv>:",

             R"(The <charconv> header in C++17 introduced the std::from_chars and std::to_chars 
functions for fast, locale-independent conversion between numbers and character sequences (e.g., strings). 
These functions avoid unnecessary memory allocation and std::string overhead, 
making them faster and more lightweight than alternatives like std::stringstream or std::stoi.)"}};
    }

    void PrintNotes() override
    {
        PrintName();
        for (const auto& pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample() override
    {
        PrintNotes();
        BasicExample();
        FloatingPointBonus();
        ErrorHandlingDemo();

        std::string str = "12345";

        auto measureTime = [](auto&& func, const std::string& str)
        {
            auto start = std::chrono::high_resolution_clock::now();
            func(str);
            auto end = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        };

        auto fromCharsTime =
            measureTime([this](const std::string& s) { FromCharsTimeTest(s); }, str);
        auto stoiTime = measureTime([this](const std::string& s) { StoiTimeTest(s); }, str);
        auto stringstreamTime =
            measureTime([this](const std::string& s) { StringstreamTimeTest(s); }, str);

        auto minTime = std::min({fromCharsTime, stoiTime, stringstreamTime});

        auto maxTime = std::max({fromCharsTime, stoiTime, stringstreamTime});

        auto printTimeWithColor = [](const char* name, auto time, auto minTime, auto maxTime)
        {
            if (time == minTime)
            {
                std::cout << "\033[32m" << name << ": \033[1m" << time << " ns\033[0m" << std::endl;
            }
            else if (time == maxTime)
            {
                std::cout << "\033[31m" << name << ": \033[1m" << time << " ns\033[0m" << std::endl;
            }
            else
            {
                std::cout << "\033[33m" << name << ": \033[1m" << time << " ns\033[0m" << std::endl;
            }
        };

        printTimeWithColor("stringstream", stringstreamTime, minTime, maxTime);
        printTimeWithColor("stoi", stoiTime, minTime, maxTime);
        printTimeWithColor("from_chars", fromCharsTime, minTime, maxTime);
    }

    void ShowDemo() override
    {
        ShowExample();
    }

   private:
    void FromCharsTimeTest(const std::string& str)
    {
        int value;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
        if (ec != std::errc())
        {
            std::cerr << "from_chars failed\n";
        }
    }

    void StoiTimeTest(const std::string& str)
    {
        try
        {
            std::stoi(str);
        }
        catch (const std::exception& e)
        {
            std::cerr << "stoi failed: " << e.what() << "\n";
        }
    }

    void StringstreamTimeTest(const std::string& str)
    {
        int value;
        std::stringstream ss(str);
        if (!(ss >> value))
        {
            std::cerr << "stringstream failed\n";
        }
    }

    void BasicExample()
    {
        std::string str = "12345";
        int value = 0;

        // Convert string to int
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

        if (ec == std::errc())
        {
            std::cout << "Converted value: " << value << '\n';
        }
        else
        {
            std::cout << "Conversion failed\n";
        }
    }

    // C++20 feature !!!
    void FloatingPointBonus()
    {
        std::array<char, 32> buffer;
        double value = 3.14159;

        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);

        if (ec == std::errc())
        {
            std::cout << "Converted string: " << std::string(buffer.data(), ptr) << '\n';
        }
    }

    void ErrorHandlingDemo()
    {
        int value;
        // Invalid numeric input (this will cause an invalid_argument error)
        const char* input = "abc";  // Invalid number string
        // Parse the string to an integer
        auto [ptr, ec] = std::from_chars(input, input + 3, value);

        // Check for an error in parsing
        if (ec == std::errc::invalid_argument)
        {
            std::cerr << "Error: Invalid number format!" << std::endl;
        }
        else
        {
            std::cout << "Parsed value: " << value << std::endl;
        }
    }
};