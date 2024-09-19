#pragma once

#include "Common/Common.hpp"
#include "MyUniquePtr.hpp"

#include <iostream>
#include <algorithm>
#include <vector>

void MyUniquePtrDemo()
{
    LOG_START_FUNCTION();

    class TmpClass
    {
        int mInt;

    public:
        TmpClass(int aInt) : mInt(aInt)
        {
            std::cout << "Instance creation :mInt :" << mInt << std::endl;
        };
        int DValue()
        {
            return mInt;
        }

        bool operator==(const TmpClass &other) const noexcept
        {
            return mInt == other.mInt;
        }
    };

    std::vector<TmpClass> TmpClassVec{TmpClass(1), TmpClass(2), TmpClass(2), TmpClass(3)};

    TmpClassVec.erase(std::remove(std::begin(TmpClassVec), std::end(TmpClassVec), TmpClass(2)), std::end(TmpClassVec));

    for (auto &value : TmpClassVec)
    {
        std::cout << "Value:" << value.DValue() << std::endl;
    }
    LOG_END_FUNCTION();
}

class PtrDemo : public DemoBase
{
public:
    PtrDemo()
    {
        mName = "PtrDemo";
        mNotes = {};
    };
    ~PtrDemo() = default;

    void ShowExample() override
    {
        PrintNotes();
        MyUniquePtrDemo();
    }

    void
    ShowDemo() override
    {
        ShowExample();
    };
};