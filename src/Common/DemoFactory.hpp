#pragma once

#include "Common/DemoBase.hpp"
#include "Common/Common.hpp"

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
#include "DeepShallowCopy.hpp"
#include "WeakPtrVsSharedPtr.hpp"
#include "ExceptionAndStack.hpp"
#include "CompilationProcess.hpp"
#include "UniquePtrFactory.hpp"
#include "MoreAboutSharedPtr.hpp"
#include "HowToWeakPtr.hpp"
#include "AllocateShared.hpp"

#include "Patterns/Pimpl.hpp"

#include <memory>

enum class Demos : int
{
    DeductingTypesDemo,
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
    Count
};

class Demofactory
{
public:
    static std::unique_ptr<DemoBase> createDemo(Demos chosenDemo)
    {
        std::unique_ptr<DemoBase> demo;

        switch (chosenDemo)
        {
        case Demos::DeductingTypesDemo:
            // Code for DeductingTypesDemo
            break;
        case Demos::AutoDeductionDemo:
            // Code for AutoDeductionDemo
            break;
        case Demos::DeclareTypeDemo:
            // Code for DeclareTypeDemo
            break;
        case Demos::UndesiredTypesDemo:
            // Code for UndesiredTypesDemo
            break;
        case Demos::AssigmentDemo:
            // Code for AssigmentDemo
            break;
        case Demos::OverloadingAndOverridingDemo:
            // Code for OverloadingAndOverridingDemo
            break;
        case Demos::PtrDemo:
            // Code for PtrDemo
            break;
        case Demos::ConstIteratorsDemo:
            // Code for ConstIteratorsDemo
            break;
        case Demos::ThrowAndNoExceptDemo:
            // Code for ThrowAndNoExceptDemo
            break;
        case Demos::ConstExpressionDemo:
            // Code for ConstExpressionDemo
            break;
        case Demos::ConstMutableDemo:
            // Code for ConstMutableDemo
            break;
        case Demos::RuleOfThreeDemo:
            // Code for RuleOfThreeDemo
            break;
        case Demos::SOLID_Demo:
            // Code for SOLID_Demo
            break;
        case Demos::VirtualDestructorDemo:
            // Code for VirtualDestructorDemo
            break;
        case Demos::ReferencePolymorphismDemo:
            // Code for ReferencePolymorphismDemo
            break;
        case Demos::StaticMemoryAllocationDemo:
            // Code for StaticMemoryAllocationDemo
            break;
        case Demos::DeepShallowCopyDemo:
            // Code for DeepShallowCopyDemo
            break;
        case Demos::WeakPtrVsSharedPtrDemo:
            // Code for WeakPtrVsSharedPtrDemo
            break;
        case Demos::ExceptionAndStackDemo:
            // Code for ExceptionAndStackDemo
            break;
        case Demos::CompilationProcessDemo:
            // Code for CompilationProcessDemo
            break;
        case Demos::UniquePtrFactoryDemo:
            // Code for UniquePtrFactoryDemo
            break;
        case Demos::MoreAboutSharedPtrDemo:
            // Code for MoreAboutSharedPtrDemo
            break;
        case Demos::HowToWeakPtrDemo:
            // Code for HowToWeakPtrDemo
            break;
        case Demos::AllocateSharedDemo:
            // Code for AllocateSharedDemo
            break;
        case Demos::PimplDemo:
        demo = std::make_unique<PimplExample::PimplExample>();
            // Code for PimplDemo
            break;
        default:
            // Code for unknown demo
            break;
        }

        return demo;
    }
};