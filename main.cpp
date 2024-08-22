#include <iostream>

#include "DeductingTypes.hpp"
#include "AutoDeduction.hpp"
#include "Declaretype.hpp"
#include "UndesiredTypes.hpp"
#include "AssigmentAndInitialization.hpp"
#include "OverloadingAndOverriding.hpp"
#include "PtrDemo.hpp"
#include "ConstIterators.hpp"
#include "ThrowAndNoExcept.hpp"

#include <memory>
#include <vector>

using namespace std;

std::vector<void (*)()> Demos =
    {
        &DeductingTypesDemo,
        &AutoDeductionDemo,
        &DeclareTypeDemo,
        &UndesiredTypesDemo,
        &AssigmentDemo,
        &OverloadingAndOverridingDemo,
        &PtrDemo,
        &ConstIteratorsDemo,
        &ThrowAndNoExceptDemo};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please select demo from  1 to " << Demos.size() << "." << std::endl;
        return 1;
    }

    int num = -1;
    try
    {
        num = std::stoi(argv[1]);
    }
    catch (std::invalid_argument exception)
    {
        std::cerr << "Wrong number." << "\nPlease pass numeric value." << std::endl;
        return 1;
    }

    if (static_cast<unsigned long>(num) >= Demos.size())
    {
        std::cerr << "Wrong number." << "\nPlease select demo from  0 to " << Demos.size() - 1 << "." << std::endl;
        return 1;
    }

    cout << "- - - - - - - - - Modern Cpp - - - - - - - - - - - \n \n";
    Demos[num]();
    return 0;
}