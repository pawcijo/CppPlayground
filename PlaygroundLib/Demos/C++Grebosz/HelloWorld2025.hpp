#include "Common/DemoBase.hpp"

#if CPP23_ENABLED
#include <print>
#endif

class HelloWorld2025Demo : public DemoBase
{
public:
    HelloWorld2025Demo()
    {
        mName = "HelloWorld 2025";
        mNotes = {{"Hello World 2025", R"(A simple demonstration of the new print library in C++2025.)"}};
    }
    
    void PrintNotes() override
    {
        PrintName();
        for (const auto& pair : mNotes)
        {
            //ptinln vs std::cout
            #ifdef __APPLE__
            #else
            std::println( "std::println:" BOLD_TEXT_START "{}" TEXT_FORMAT_CLEAR "\n{}\n", pair.first, pair.second);
            #endif

            std::cout << "\033[31mThis is red text\033[0m" << std::endl;
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
    {
        if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);
            // Also print to console to show functionality
            PrintNotes();
        }
    }

    void ShowDemo(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
    {
        ShowExample(printNotesCallback);
    }


};  
