#pragma once

#include "Common/Common.hpp"
#include <iostream>

namespace DeepShallowCopy
{

    constexpr const char *ShallowNote = "A shallow copy duplicates the object's values exactly as they are,\n"
                                        "but does not create a new copy of dynamically allocated memory (such as data pointed to by pointers).\n"
                                        "Instead, both the original object and the copied object will point to the same memory location.\n";

    constexpr const char *DeepNote = "A deep copy duplicates not only the object's values but also creates new copies of dynamically\n"
                                     "allocated memory or other referenced objects.\n";

    void PrintNote(const char *note)
    {
        std::cout << note << std::endl;
    }

    // This only need shallow copy
    class ShallowExample
    {
        int value;

    public:
        ShallowExample() : value(5) {};

        ShallowExample(ShallowExample &another)
        {
            value = another.value;
            std::cout << "Shallow copy run" << std::endl;
        };
    };

    class DeepExample
    {
        int value;
        double *dynamicMember;

    public:
        DeepExample() : value(5), dynamicMember(new double(5)) {};

        double *DynamicMemberAddress() { return dynamicMember; }

        DeepExample(const DeepExample &another)
        {
            value = another.value;
            dynamicMember = new double(*another.dynamicMember);
            std::cout << "Deep copy constructor" << std::endl;
        }

        DeepExample operator=(const DeepExample &another)
        {
            value = another.value;
            dynamicMember = new double(*another.dynamicMember);

            std::cout << "Deep copy assign operator" << std::endl;
            return *this;
        };
    };

    void ShallowCopyExample()
    {
        LOG_START_FUNCTION();

        ShallowExample obj;
        ShallowExample obj2 = obj;

        LOG_END_FUNCTION();
    }

    void DeepCopyExample()
    {
        LOG_START_FUNCTION();

        DeepExample obj;
        DeepExample obj2 = obj;

        Show_address("obj",*obj.DynamicMemberAddress());
        Show_address("obj2",*obj2.DynamicMemberAddress());

        LOG_END_FUNCTION();
    }

    void DeepShallowCopyDemo()
    {
        PrintNote(ShallowNote);
        ShallowCopyExample();

        PrintNote(DeepNote);
        DeepCopyExample();
    }

}