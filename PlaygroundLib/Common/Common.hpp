#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <functional>

#define LOG_FUNCTION_NAME(color) printf("\n\033[1;%sm%s:\033[0m\n", color, __FUNCTION__)

#define LOG_START_FUNCTION() \
    printf("\n%s: - - - - - - - - - -  Start - - - - - - - - -\n", __FUNCTION__)

#define LOG_END_FUNCTION() \
    printf("%s: - - - - - - - - - -  End - - - - - - - - - -\n\n", __FUNCTION__)

#define BOLD_TEXT_START "\033[1;37m"
#define BOLD_RED_BOLD_TEXT_START "\033[1;31m"

#define ITALIC_TEXT_START "\033[3;37m"
#define BOLD_ITALIC_TEXT_START "\033[1;3m"

#define TEXT_FORMAT_CLEAR "\033[0m"

#define YELLOW_BOLD_TEXT_START "\033[1;33m"
#define BLUE_BOLD_TEXT_START "\033[1;34m"
#define RED_BOLD_TEXT_START "\033[1;31m"
#define CYAN_BOLD_TEXT_START "\033[1;36m"
#define GREEN_BOLD_TEXT_START "\033[1;32m"

#define YELLOW_TEXT_START "\033[33m"
#define BLUE_TEXT_START "\033[34m"
#define RED_TEXT_START "\033[31m"
#define CYAN_TEXT_START "\033[36m"
#define GREEN_TEXT_START "\033[32m"


#define YELLOW "33"
#define BLUE "34"
#define RED "31"
#define CYAN "36"
#define GREEN "32"
#define WHITE "37"

#define TEXT_COLOR_CLEAR "\033[0m"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define CLASS_NAME() (typeid(*this).name())

using NoteFormat = std::vector<std::pair<std::string, std::string>>;

inline bool is_string_only_numeric(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

// Overload for lvalues
template <typename T>
void detectValueCategory(T &arg)
{
    if (std::is_const<std::remove_reference_t<T>>::value)
    {
        std::cout << "Argument is a const lvalue. (" << typeid(arg).name() << ")" << std::endl;
    }
    else
    {
        std::cout << "Argument is a non-const lvalue. (" << typeid(arg).name() << ")" << std::endl;
    }
}

// Overload for rvalues
// or universal reference.
template <typename T>
void detectValueCategory(T &&arg)
{
    if (std::is_const<std::remove_reference_t<T>>::value)
    {
        std::cout << " a const rvalue. (" << typeid(arg).name() << ")" << std::endl;
    }
    else
    {
        std::cout << " non-const rvalue. (" << typeid(arg).name() << ")" << std::endl;
    }
}

// Helper function to distinguish between references and value types
template <typename T>
void detectValueCategoryImpl(T &&arg)
{
    // Check if the argument is a reference
    if (std::is_reference<T>::value)
    {
        std::cout << "Argument is a reference and ";
        detectValueCategory(std::forward<T>(arg));
    }
    else
    {
        std::cout << "Argument is not a reference and ";
        // For non-references, we need to handle universal references and rvalues separately.
        detectValueCategory(std::forward<T>(arg));
    }
}

template <typename T>
void Show_address(T &param)
{
    std::cout << "Address of value: " << std::addressof(param) << std::endl;
}

template <typename T>
void Show_address(const char *name, T &param)
{
    std::cout << "Address of " << name << ":" << std::addressof(param) << std::endl;
}

struct Timer
{
    const char *funcName;
    std::chrono::high_resolution_clock::time_point start;

    Timer(const char *name) : funcName(name), start(std::chrono::high_resolution_clock::now())
    {
    }

    ~Timer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Time taken by " << funcName << ": " << duration << " ms\n";
    }
};

#define TIME_FUNCTION Timer _timer(__func__)


template <typename Func, typename... Args>
auto MeasureTime(Func&& func, Args&&... args) {
    using ReturnType = decltype(func(std::forward<Args>(args)...)); // Get function return type

    auto start = std::chrono::high_resolution_clock::now();
    ReturnType result = func(std::forward<Args>(args)...);  // Execute function
    auto end = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    return std::make_pair(result, elapsed);
}
