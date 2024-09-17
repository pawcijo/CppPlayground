#pragma once

#include "Common/Common.hpp"

#include <string>
#include <memory>

namespace AllocateShared
{

    NoteFormat notes = {{{"Custom Allocator: "}, {R"(std::make_unique and std::make_shared are two of the three make functions:
functions that take an arbitrary set of arguments, perfect-forward them to the constructor
for a dynamically allocated object, and return a smart pointer to that object.
The third make function is std::allocate_shared. It acts just like
std::make_shared, except its first argument is an allocator object to be used for the
dynamic memory allocation.)"}}};

    void PrintNotes()
    {
        for (const auto &pair : notes)
        {
            std::cout << BOLD_TEXT_START << pair.first << BOLD_TEXT_END << std::endl
                      << pair.second << std::endl<<std::endl;
        }
    }

    class TestClass
    {
        std::string someString;

    public:
        TestClass() : someString("ExampleString")
        {
            std::cout << "Random obj created." << std::endl;
        };

        TestClass(const TestClass &another) = default;

        TestClass operator=(const TestClass &another)
        {
            someString = another.someString;

            return *this;
        }

        TestClass(const TestClass &&another)
        {
            someString = another.someString;
        };

        TestClass operator=(const TestClass &&another)
        {
            someString = another.someString;

            return *this;
        }

        std::string stringValue() const { return someString; }
    };


template <typename T>
struct CustomAllocator : public std::allocator<T> {
    using Base = std::allocator<T>;
    using value_type = typename Base::value_type;

    // Constructor
    CustomAllocator() = default;

    // Copy constructor
    template <typename U>
    CustomAllocator(const CustomAllocator<U>& other) : Base(other) {}

    // Override allocate to add custom behavior
    T* allocate(std::size_t n) {
        std::cout << "Custom allocate: Allocating " << n << " objects of size " << sizeof(T) << "\n";
        return Base::allocate(n);  // Use std::allocator's allocate
    }

    // Override deallocate to add custom behavior
    void deallocate(T* p, std::size_t n) {
        std::cout << "Custom deallocate: Deallocating " << n << " objects of size " << sizeof(T) << "\n";
        Base::deallocate(p, n);  // Use std::allocator's deallocate
    }

    // The construct and destroy methods are inherited from std::allocator and don't need to be overridden unless custom behavior is required.
};

    // Equality comparison for allocators
    template <typename T, typename U>
    bool operator==(const CustomAllocator<T> &, const CustomAllocator<U> &) { return true; }

    template <typename T, typename U>
    bool operator!=(const CustomAllocator<T> &, const CustomAllocator<U> &) { return false; }

    void AllocateSharedExample()
    {
        LOG_START_FUNCTION();
        PrintNotes();
        CustomAllocator<TestClass> alloc;
        auto obj = std::allocate_shared<TestClass>(alloc);

                LOG_END_FUNCTION();
    }

    void AllocateSharedDemo()
    {
        AllocateSharedExample();
    }
}