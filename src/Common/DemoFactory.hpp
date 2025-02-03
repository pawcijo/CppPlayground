#pragma once

#include <map>
#include <memory>
#include <string>
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
#include "Demos/EmptyDemo.hpp"
#include "Demos/ExceptionAndStack.hpp"
#include "Demos/ForwardValue.hpp"
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

// C++ 17
#include "Demos/CPP17/AnyDemo.hpp"
#include "Demos/CPP17/VariantDemo.hpp"
#include "Demos/CPP17/StringViewDemo.hpp"

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
    ForwardDemo,
    EmptyDemo,
    AnyDemo,
    VariantDemo,
    StringViewDemo,
    Count
};

#define ENUM_TO_STRING(name) #name
/*
This is also design pattern. :)
Abstract factory as BaseDemo is form of abstraction.
*/
class Demofactory
{
   public:
    using DemoCreator = std::function<std::unique_ptr<DemoBase>()>;
    using ValuePair = std::pair<DemoCreator, std::string>;

    static std::unique_ptr<DemoBase> createDemo(Demos chosenDemo)
    {
        const auto& it = getDemoMap().find(chosenDemo);
        if (it != getDemoMap().end())
        {
            return it->second.first();  // Call the associated lambda function to create the object
        }
        return nullptr;
    }

    static const std::map<Demos, ValuePair>& getDemoMap()
    {
        // Initialize the map statically
        static const std::map<Demos, ValuePair> demoMap = {
            {Demos::DeductingTypesDemo,
             ValuePair([]() { return std::make_unique<DeductingTypesDemo>(); },
                       "DeductingTypesDemo")},
            {Demos::AutoDeductionDemo,
             ValuePair([]() { return std::make_unique<AutoDeductionDemo>(); },
                       "AutoDeductionDemo")},
            {Demos::DeclareTypeDemo,
             ValuePair([]() { return std::make_unique<DeclareTypeDemo>(); }, "DeclareTypeDemo")},
            {Demos::UndesiredTypesDemo,
             ValuePair([]() { return std::make_unique<UndesiredTypesDemo>(); },
                       "UndesiredTypesDemo")},
            {Demos::AssigmentDemo,
             ValuePair([]() { return std::make_unique<AssigmentDemo>(); }, "AssigmentDemo")},
            {Demos::OverloadingAndOverridingDemo,
             ValuePair([]() { return std::make_unique<OverloadingAndOverridingDemo>(); },
                       "OverloadingAndOverridingDemo")},
            {Demos::PtrDemo, ValuePair([]() { return std::make_unique<PtrDemo>(); }, "PtrDemo")},
            {Demos::ConstIteratorsDemo,
             ValuePair([]() { return std::make_unique<ConstIteratorsDemo>(); },
                       "ConstIteratorsDemo")},
            {Demos::ThrowAndNoExceptDemo,
             ValuePair([]() { return std::make_unique<ThrowAndNoExceptDemo>(); },
                       "ThrowAndNoExceptDemo")},
            {Demos::ConstExpressionDemo,
             ValuePair([]() { return std::make_unique<ConstExpressionDemo>(); },
                       "ConstExpressionDemo")},
            {Demos::ConstMutableDemo,
             ValuePair([]() { return std::make_unique<ConstMutableDemo>(); }, "ConstMutableDemo")},
            {Demos::RuleOfThreeDemo,
             ValuePair([]() { return std::make_unique<RuleOfThreeDemo>(); }, "RuleOfThreeDemo")},
            {Demos::SOLID_Demo,
             ValuePair([]() { return std::make_unique<SOLID_Demo>(); }, "SOLID_Demo")},
            {Demos::VirtualDestructorDemo,
             ValuePair([]() { return std::make_unique<VirtualDestructorDemo>(); },
                       "VirtualDestructorDemo")},
            {Demos::ReferencePolymorphismDemo,
             ValuePair([]() { return std::make_unique<ReferencePolymorphismDemo>(); },
                       "ReferencePolymorphismDemo")},
            {Demos::StaticMemoryAllocationDemo,
             ValuePair([]() { return std::make_unique<StaticMemoryAllocationDemo>(); },
                       "StaticMemoryAllocationDemo")},
            {Demos::DeepShallowCopyDemo,
             ValuePair([]() { return std::make_unique<DeepShallowCopyDemo>(); },
                       "DeepShallowCopyDemo")},
            {Demos::WeakPtrVsSharedPtrDemo,
             ValuePair([]() { return std::make_unique<WeakPtrVsSharedPtrDemo>(); },
                       "WeakPtrVsSharedPtrDemo")},
            {Demos::ExceptionAndStackDemo,
             ValuePair([]() { return std::make_unique<ExceptionAndStackDemo>(); },
                       "ExceptionAndStackDemo")},
            {Demos::CompilationProcessDemo,
             ValuePair([]() { return std::make_unique<CompilationProcessDemo>(); },
                       "CompilationProcessDemo")},
            {Demos::UniquePtrFactoryDemo,
             ValuePair([]() { return std::make_unique<UniquePtrFactoryDemo>(); },
                       "UniquePtrFactoryDemo")},
            {Demos::MoreAboutSharedPtrDemo,
             ValuePair([]() { return std::make_unique<MoreAboutSharedPtrDemo>(); },
                       "MoreAboutSharedPtrDemo")},
            {Demos::HowToWeakPtrDemo,
             ValuePair([]() { return std::make_unique<HowToWeakPtrDemo>(); }, "HowToWeakPtrDemo")},
            {Demos::AllocateSharedDemo,
             ValuePair([]() { return std::make_unique<AllocateSharedDemo>(); },
                       "AllocateSharedDemo")},
            {Demos::PimplDemo,
             ValuePair([]() { return std::make_unique<PimplExample>(); }, "PimplDemo")},
            {Demos::UniversalRefVsRValueRefDemo,
             ValuePair([]() { return std::make_unique<UniversalRefVsRValueRefDemo>(); },
                       "UniversalRefVsRValueRefDemo")},
            {Demos::ForwardDemo,
             ValuePair([]() { return std::make_unique<ForwardDemo>(); }, "ForwardDemo")},
            {Demos::EmptyDemo,
             ValuePair([]() { return std::make_unique<EmptyDemo>(); }, "EmptyDemo")},
            {Demos::AnyDemo, ValuePair([]() { return std::make_unique<AnyDemo>(); }, "AnyDemo")},
            {Demos::VariantDemo,
             ValuePair([]() { return std::make_unique<VariantDemo>(); }, "VariantDemo")},
             {Demos::StringViewDemo,ValuePair([]() { return std::make_unique<StringViewDemo>(); }, "StringViewDemo")
        }};
        return demoMap;
    }
};