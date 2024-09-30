#pragma once

#include <map>
#include <memory>
#include <utility>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include "Demos/AllocateShared.hpp"
#include "Demos/AssigmentAndInitialization.hpp"
#include "Demos/AutoDeduction.hpp"
#include "Demos/CompilationProcess.hpp"
#include "Demos/ConstExpreession.hpp"
#include "Demos/ConstIterators.hpp"
#include "Demos/ConstMutable.hpp"
#include "Demos/Declaretype.hpp"
#include "Demos/DeductingTypes.hpp"
#include "Demos/DeepShallowCopy.hpp"
#include "Demos/ExceptionAndStack.hpp"
#include "Demos/HowToWeakPtr.hpp"
#include "Demos/MoreAboutSharedPtr.hpp"
#include "Demos/OverloadingAndOverriding.hpp"
#include "Demos/Patterns/Pimpl.hpp"
#include "Demos/PtrDemo.hpp"
#include "Demos/ReferencePolymorphism.hpp"
#include "Demos/RuleOfThree.hpp"
#include "Demos/SOLID.hpp"
#include "Demos/StaticMemoryAllocation.hpp"
#include "Demos/ThrowAndNoExcept.hpp"
#include "Demos/UndesiredTypes.hpp"
#include "Demos/UniquePtrFactory.hpp"
#include "Demos/UniversalRefVsRValueRef.hpp"
#include "Demos/VirtualDestructor.hpp"
#include "Demos/WeakPtrVsSharedPtr.hpp"

enum class Demos : int
{
    DeductingTypesDemo = 1,
    AutoDeductionDemo,
    DeclareTypeDemo,
    UndesiredTypesDemo,
    AssigmentDemo,
    OverloadingAndOverridingDemo,
    PtrDemo,
    ConstIteratorsDemo,
    ThrowAndNoExceptDemo,
    ConstExpressionDemo,
    ConstMutableDemo,
    RuleOfThreeDemo,
    SOLID_Demo,
    VirtualDestructorDemo,
    ReferencePolymorphismDemo,
    StaticMemoryAllocationDemo,
    DeepShallowCopyDemo,
    WeakPtrVsSharedPtrDemo,
    ExceptionAndStackDemo,
    CompilationProcessDemo,
    UniquePtrFactoryDemo,
    MoreAboutSharedPtrDemo,
    HowToWeakPtrDemo,
    AllocateSharedDemo,
    PimplDemo,
    UniversalRefVsRValueRefDemo,
    Count
};

#define ENUM_TO_STRING(name) #name

const std::vector<std::pair<Demos, std::string>> demoVector = {
    {Demos::DeductingTypesDemo, "DeductingTypesDemo"},
    {Demos::AutoDeductionDemo, "AutoDeductionDemo"},
    {Demos::DeclareTypeDemo, "DeclareTypeDemo"},
    {Demos::UndesiredTypesDemo, "UndesiredTypesDemo"},
    {Demos::AssigmentDemo, "AssigmentDemo"},
    {Demos::OverloadingAndOverridingDemo, "OverloadingAndOverridingDemo"},
    {Demos::PtrDemo, "PtrDemo"},
    {Demos::ConstIteratorsDemo, "ConstIteratorsDemo"},
    {Demos::ThrowAndNoExceptDemo, "ThrowAndNoExceptDemo"},
    {Demos::ConstExpressionDemo, "ConstExpressionDemo"},
    {Demos::ConstMutableDemo, "ConstMutableDemo"},
    {Demos::RuleOfThreeDemo, "RuleOfThreeDemo"},
    {Demos::SOLID_Demo, "SOLID_Demo"},
    {Demos::VirtualDestructorDemo, "VirtualDestructorDemo"},
    {Demos::ReferencePolymorphismDemo, "ReferencePolymorphismDemo"},
    {Demos::StaticMemoryAllocationDemo, "StaticMemoryAllocationDemo"},
    {Demos::DeepShallowCopyDemo, "DeepShallowCopyDemo"},
    {Demos::WeakPtrVsSharedPtrDemo, "WeakPtrVsSharedPtrDemo"},
    {Demos::ExceptionAndStackDemo, "ExceptionAndStackDemo"},
    {Demos::CompilationProcessDemo, "CompilationProcessDemo"},
    {Demos::UniquePtrFactoryDemo, "UniquePtrFactoryDemo"},
    {Demos::MoreAboutSharedPtrDemo, "MoreAboutSharedPtrDemo"},
    {Demos::HowToWeakPtrDemo, "HowToWeakPtrDemo"},
    {Demos::AllocateSharedDemo, "AllocateSharedDemo"},
    {Demos::PimplDemo, "PimplDemo"},
    {Demos::UniversalRefVsRValueRefDemo, "UniversalRefVsRValueRefDemo"}};

class Demofactory
{
   public:
    static std::unique_ptr<DemoBase> createDemo(Demos chosenDemo)
    {
        std::unique_ptr<DemoBase> demo;

        switch (chosenDemo)
        {
            case Demos::DeductingTypesDemo:
                demo = std::make_unique<DeductingTypesDemo>();
                break;
            case Demos::AutoDeductionDemo:
                demo = std::make_unique<AutoDeductionDemo>();
                break;
            case Demos::DeclareTypeDemo:
                demo = std::make_unique<DeclareTypeDemo>();
                break;
            case Demos::UndesiredTypesDemo:
                demo = std::make_unique<UndesiredTypesDemo>();
                break;
            case Demos::AssigmentDemo:
                demo = std::make_unique<AssigmentDemo>();
                break;
            case Demos::OverloadingAndOverridingDemo:
                demo = std::make_unique<OverloadingAndOverridingDemo>();
                break;
            case Demos::PtrDemo:
                demo = std::make_unique<PtrDemo>();
                break;
            case Demos::ConstIteratorsDemo:
                demo = std::make_unique<ConstIteratorsDemo>();
                break;
            case Demos::ThrowAndNoExceptDemo:
                demo = std::make_unique<ThrowAndNoExceptDemo>();
                break;
            case Demos::ConstExpressionDemo:
                demo = std::make_unique<ConstExpressionDemo>();
                break;
            case Demos::ConstMutableDemo:
                demo = std::make_unique<ConstMutableDemo>();
                break;
            case Demos::RuleOfThreeDemo:
                demo = std::make_unique<RuleOfThreeDemo>();
                break;
            case Demos::SOLID_Demo:
                demo = std::make_unique<SOLID_Demo>();
                break;
            case Demos::VirtualDestructorDemo:
                demo = std::make_unique<VirtualDestructorDemo>();
                break;
            case Demos::ReferencePolymorphismDemo:
                demo = std::make_unique<ReferencePolymorphismDemo>();
                break;
            case Demos::StaticMemoryAllocationDemo:
                demo = std::make_unique<StaticMemoryAllocationDemo>();
                break;
            case Demos::DeepShallowCopyDemo:
                demo = std::make_unique<DeepShallowCopyDemo>();
                break;
            case Demos::WeakPtrVsSharedPtrDemo:
                demo = std::make_unique<WeakPtrVsSharedPtrDemo>();
                break;
            case Demos::ExceptionAndStackDemo:
                demo = std::make_unique<ExceptionAndStackDemo>();
                break;
            case Demos::CompilationProcessDemo:
                demo = std::make_unique<CompilationProcessDemo>();
                break;
            case Demos::UniquePtrFactoryDemo:
                demo = std::make_unique<AutoDeductionDemo>();
                break;
            case Demos::MoreAboutSharedPtrDemo:
                demo = std::make_unique<MoreAboutSharedPtrDemo>();
                break;
            case Demos::HowToWeakPtrDemo:
                demo = std::make_unique<HowToWeakPtrDemo>();
                break;
            case Demos::AllocateSharedDemo:
                demo = std::make_unique<AllocateSharedDemo>();
                break;
            case Demos::PimplDemo:
                demo = std::make_unique<PimplExample>();
                break;
            case Demos::UniversalRefVsRValueRefDemo:
                demo = std::make_unique<UniversalRefVsRValueRefDemo>();
                break;
            default:
                // Code for unknown demo
                break;
        }

        return demo;
    }
};