#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <utility>
#include <string>
#include <vector>
#include <iostream>

const std::vector<std::pair<std::string, std::string>> Notes = {
    {{"try block"}, {"Code that may potentially throw an exception is placed inside a try block."}},
    {{"throw"}, {"If an error occurs, an exception is 'thrown' using the throw keyword."}},
    {{"catch block"}, {"The thrown exception is caught using a catch block, where it is handled appropriately.\n"}}};

const std::vector<std::pair<std::string, std::string>> StackUnwindingNotes =
    {{{"Stack Unwinding"}, {"When an exception is thrown, the control is transferred to the nearest catch block.\n"
                            "However, before the program reaches the catch block, the destructors of all local objects in the current\n"
                            "scope are called (this is the 'unwinding' part). \n"
                            "This ensures that resources like memory or file handles are properly released,\n"
                            "avoiding memory leaks or resource leaks, even when an error occurs.\n"}}};

void PrintNote()
{
    for (const auto &pair : Notes)
    {
        std::cout << BOLD_TEXT_START << pair.first << ": " << BOLD_TEXT_END << pair.second << std::endl;
    }
}

void PrintStackUnwindingNotes()
{
    for (const auto &pair : StackUnwindingNotes)
    {
        std::cout << BOLD_TEXT_START << pair.first << ": " << BOLD_TEXT_END << pair.second << std::endl;
    }
}

class Test
{
public:
    Test()
    {
        std::cout << "Object created" << std::endl;
    }
    ~Test()
    {
        std::cout << "Object destroyed" << std::endl;
    }
};

void func()
{
    Test obj; // Object will be destroyed during stack unwinding if exception occurs
    throw std::logic_error("An error occurred!");
}

void ExceptionAndStackExample() noexcept
{
    try
    {
        func();
    }
    catch (std::logic_error &e)
    {
        std::cout << "Caught exception: " << *e.what() << std::endl
                  << std::endl;
    }
}

void ExceptionAndStackExampleNotCatch()
{
    func();
}

class ExceptionAndStackDemo : public DemoBase
{
public:
    ExceptionAndStackDemo()
    {
        mName = "ExceptionAndStackDemo";
        mNotes = {};
    };
    ~ExceptionAndStackDemo() = default;

    void ShowExample() override
    {
        PrintNotes();
        PrintNote();
        ExceptionAndStackExample();

        PrintStackUnwindingNotes();
        ExceptionAndStackExampleNotCatch();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
