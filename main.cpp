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
#include "ConstExpreession.hpp"
#include "ConstMutable.hpp"
#include "RuleOfThree.hpp"
#include "SOLID.hpp"
#include "VirtualDestructor.hpp"
#include "ReferencePolymorphism.hpp"
#include "StaticMemoryAllocation.hpp"

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
        &ThrowAndNoExceptDemo,
        &ConstExpressionDemo,
        &ConstMutable::ConstMutableDemo,
        &RuleOfThree::RuleOfThreeDemo,
        &SOLID::SOLID_Demo,
        &VirtualDestructor::VirtualDestructorDemo,
        &ReferencePoli::ReferencePolymorphismDemo,
        &StaticMemoryAllocation::StaticMemoryAllocationDemo};

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

    cout << "\033[1;32m- - - - - - - - -Modern Cpp- - - - - - - - - - - \033[0m\n"<<std::endl;
    Demos[num]();
    return 0;
}