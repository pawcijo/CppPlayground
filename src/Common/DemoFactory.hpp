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
    Count
};

#define ENUM_TO_STRING(name) #name

class Demofactory {
public:
    using DemoCreator = std::unique_ptr<DemoBase>(*)();
     using ValuePair = std::pair<DemoCreator,std::string>;

    static std::unique_ptr<DemoBase> createDemo(Demos chosenDemo) {
        const auto& it = getDemoMap().find(chosenDemo);
        if (it != getDemoMap().end()) {
            return it->second.first();  // Call the associated lambda function to create the object
        }
        return nullptr;
    }

    static std::unique_ptr<DemoBase> createDeductingTypesDemo() { return std::make_unique<DeductingTypesDemo>(); }
    static std::unique_ptr<DemoBase> createAutoDeductionDemo() { return std::make_unique<AutoDeductionDemo>(); }
    static std::unique_ptr<DemoBase> createDeclareTypeDemo() { return std::make_unique<DeclareTypeDemo>(); }
    static std::unique_ptr<DemoBase> createUndesiredTypesDemo() { return std::make_unique<UndesiredTypesDemo>(); }
    static std::unique_ptr<DemoBase> createAssigmentDemo() { return std::make_unique<AssigmentDemo>(); }
    static std::unique_ptr<DemoBase> createOverloadingAndOverridingDemo() { return std::make_unique<OverloadingAndOverridingDemo>(); }
    static std::unique_ptr<DemoBase> createPtrDemo() { return std::make_unique<PtrDemo>(); }
    static std::unique_ptr<DemoBase> createConstIteratorsDemo() { return std::make_unique<ConstIteratorsDemo>(); }
    static std::unique_ptr<DemoBase> createThrowAndNoExceptDemo() { return std::make_unique<ThrowAndNoExceptDemo>(); }
    static std::unique_ptr<DemoBase> createConstExpressionDemo() { return std::make_unique<ConstExpressionDemo>(); }
    static std::unique_ptr<DemoBase> createConstMutableDemo() { return std::make_unique<ConstMutableDemo>(); }
    static std::unique_ptr<DemoBase> createRuleOfThreeDemo() { return std::make_unique<RuleOfThreeDemo>(); }
    static std::unique_ptr<DemoBase> createSOLID_Demo() { return std::make_unique<SOLID_Demo>(); }
    static std::unique_ptr<DemoBase> createVirtualDestructorDemo() { return std::make_unique<VirtualDestructorDemo>(); }
    static std::unique_ptr<DemoBase> createReferencePolymorphismDemo() { return std::make_unique<ReferencePolymorphismDemo>(); }
    static std::unique_ptr<DemoBase> createStaticMemoryAllocationDemo() { return std::make_unique<StaticMemoryAllocationDemo>(); }
    static std::unique_ptr<DemoBase> createDeepShallowCopyDemo() { return std::make_unique<DeepShallowCopyDemo>(); }
    static std::unique_ptr<DemoBase> createWeakPtrVsSharedPtrDemo() { return std::make_unique<WeakPtrVsSharedPtrDemo>(); }
    static std::unique_ptr<DemoBase> createExceptionAndStackDemo() { return std::make_unique<ExceptionAndStackDemo>(); }
    static std::unique_ptr<DemoBase> createCompilationProcessDemo() { return std::make_unique<CompilationProcessDemo>(); }
    static std::unique_ptr<DemoBase> createUniquePtrFactoryDemo() { return std::make_unique<AutoDeductionDemo>(); }
    static std::unique_ptr<DemoBase> createMoreAboutSharedPtrDemo() { return std::make_unique<MoreAboutSharedPtrDemo>(); }
    static std::unique_ptr<DemoBase> createHowToWeakPtrDemo() { return std::make_unique<HowToWeakPtrDemo>(); }
    static std::unique_ptr<DemoBase> createAllocateSharedDemo() { return std::make_unique<AllocateSharedDemo>(); }
    static std::unique_ptr<DemoBase> createPimplDemo() { return std::make_unique<PimplExample>(); }
    static std::unique_ptr<DemoBase> createUniversalRefVsRValueRefDemo() { return std::make_unique<UniversalRefVsRValueRefDemo>(); }
    static std::unique_ptr<DemoBase> createForwardDemo() { return std::make_unique<ForwardDemo>(); }

public:
    static const std::map<Demos, ValuePair>& getDemoMap() 
    {
        // Initialize the map statically
        static const std::map<Demos, ValuePair> demoMap = {
            {Demos::DeductingTypesDemo,ValuePair(&createDeductingTypesDemo,"DeductingTypesDemo")},
            {Demos::AutoDeductionDemo, ValuePair(&createAutoDeductionDemo,"AutoDeductionDemo")},
            {Demos::DeclareTypeDemo, ValuePair(&createDeclareTypeDemo,"DeclareTypeDemo")},
            {Demos::UndesiredTypesDemo, ValuePair(&createUndesiredTypesDemo,"UndesiredTypesDemo")},
            {Demos::AssigmentDemo, ValuePair(&createAssigmentDemo,"AssigmentDemo")},
            {Demos::OverloadingAndOverridingDemo, ValuePair(&createOverloadingAndOverridingDemo,"OverloadingAndOverridingDemo")},
            {Demos::PtrDemo, ValuePair(&createPtrDemo,"PtrDemo")},
            {Demos::ConstIteratorsDemo, ValuePair(&createConstIteratorsDemo,"ConstIteratorsDemo")},
            {Demos::ThrowAndNoExceptDemo, ValuePair(&createThrowAndNoExceptDemo,"ThrowAndNoExceptDemo")},
            {Demos::ConstExpressionDemo, ValuePair(&createConstExpressionDemo,"ConstExpressionDemo")},
            {Demos::ConstMutableDemo, ValuePair(&createConstMutableDemo,"ConstMutableDemo")},
            {Demos::RuleOfThreeDemo, ValuePair(&createRuleOfThreeDemo,"RuleOfThreeDemo")},
            {Demos::SOLID_Demo, ValuePair(&createSOLID_Demo,"SOLID_Demo")},
            {Demos::VirtualDestructorDemo, ValuePair(&createVirtualDestructorDemo,"VirtualDestructorDemo")},
            {Demos::ReferencePolymorphismDemo, ValuePair(&createReferencePolymorphismDemo,"ReferencePolymorphismDemo")},
            {Demos::StaticMemoryAllocationDemo, ValuePair(&createStaticMemoryAllocationDemo,"StaticMemoryAllocationDemo")},
            {Demos::DeepShallowCopyDemo, ValuePair(&createDeepShallowCopyDemo,"DeepShallowCopyDemo")},
            {Demos::WeakPtrVsSharedPtrDemo, ValuePair(&createWeakPtrVsSharedPtrDemo,"WeakPtrVsSharedPtrDemo")},
            {Demos::ExceptionAndStackDemo, ValuePair(&createExceptionAndStackDemo,"ExceptionAndStackDemo")},
            {Demos::CompilationProcessDemo, ValuePair(&createCompilationProcessDemo,"CompilationProcessDemo")},
            {Demos::UniquePtrFactoryDemo, ValuePair(&createUniquePtrFactoryDemo,"UniquePtrFactoryDemo")},
            {Demos::MoreAboutSharedPtrDemo, ValuePair(&createMoreAboutSharedPtrDemo,"MoreAboutSharedPtrDemo")},
            {Demos::HowToWeakPtrDemo, ValuePair(&createHowToWeakPtrDemo,"HowToWeakPtrDemo")},
            {Demos::AllocateSharedDemo, ValuePair(&createAllocateSharedDemo,"AllocateSharedDemo")},
            {Demos::PimplDemo, ValuePair(&createPimplDemo,"PimplDemo")},
            {Demos::UniversalRefVsRValueRefDemo, ValuePair(&createUniversalRefVsRValueRefDemo,"UniversalRefVsRValueRefDemo")},
            {Demos::ForwardDemo, ValuePair(&createForwardDemo,"ForwardDemo")}
        };
        return demoMap;
    }
};