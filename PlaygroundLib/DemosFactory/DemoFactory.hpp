#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "CommonDemos.h" // IWYU pragma: keep

// Design patterns
#include "Demos/Patterns/Adapter/AdapterDemo.hpp"
#include "Demos/Patterns/Builder/BuilderDemo.hpp"
#include "Demos/Patterns/Pimpl.hpp"
#include "Demos/Patterns/Singleton/SingletonDemo.hpp"

// C++ 11
#include "Demos/CPP11/FutureObj.hpp"

// C++ 17
#include "Demos/CPP17/AnyDemo.hpp"
#include "Demos/CPP17/CharConvDemo.hpp"
#include "Demos/CPP17/ExecutionDemo.hpp"
#include "Demos/CPP17/FileSystemDemo.hpp"
#include "Demos/CPP17/MemoryResourceDemo.hpp"
#include "Demos/CPP17/OptionalDemo.hpp"
#include "Demos/CPP17/StringViewDemo.hpp"
#include "Demos/CPP17/VariantDemo.hpp"

// C++ 20
#include "Demos/CPP20/BitDemo.hpp"
#include "Demos/CPP20/CompareDemo.hpp"
#include "Demos/CPP20/CorutineDemo.hpp"
#include "Demos/CPP20/PredicateDemo.hpp"
#include "Demos/CPP20/SemaphoreDemo.hpp"

// C++ Grebosz
#include "Demos/C++Grebosz/HelloWorld2025.hpp"
#include "Demos/C++Grebosz/TypeLimits.hpp"
#include "Demos/C++Grebosz/Zadanko4_16III.hpp"
#include "Demos/C++Grebosz/Zadanko8_28.hpp"
#include "Demos/C++Grebosz/Zadanko12_5_30.hpp"
#include "Demos/C++Grebosz/Zadanko13_9_10.hpp" 
#include "Demos/C++Grebosz/Zadanko14_7_16.hpp"

// Question tasks
#include "QuestionTasks/LongestCharSequence.hpp"
#include "QuestionTasks/StringToInt.hpp"
#include "QuestionTasks/LongestUniqueSubstring.hpp"
#include "QuestionTasks/LongestPalindrom.hpp"

// Other
#include "Demos/NeuralNetwork/NeuralNetworkDemo.hpp"

enum class DemoType : int
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
  CharConvDemo,
  ExecutionDemo,
  FileSystemDemo,
  MemoryResourceDemo,
  OptionalDemo,
  NeuralNetworkDemo,
  SingletonDemo,
  BuilderDemo,
  AdapterDemo,
  BitDemo,
  SemaphoreDemo,
  CompareDemo,
  PredicateDemo,
  CorutineDemo,
  HelloWorld2025Demo,
  TypeLimitsDemo,
  OutOfRangeDemo,
  LongestCharSequenceDemo,
  StringToIntoDemo,
  LongestUniqueSubstringDemo,
  LongestPalindromDemo,
  Zadanko4_16IIIDemo,
  Zadanko8_28Demo,
  Zadanko12_5_30Demo,
  Zadanko13_9_10Demo,
  Zadanko14_7_16Demo,
  FutureObjDemo,
  Count
};

enum class DemoTag : int
{
  Common = 1,
  DesignPatterns,
  CPP11,
  CPP14,
  CPP17,
  CPP20,
  NeuralNetwork,
  GreboszCPP,
  QuestionTask,
  Count
};

#define ENUM_TO_STRING(name) #name
/*
This is also design pattern. :)
Abstract factory as BaseDemo is a form of abstraction.
*/
class DemoFactory
{
public:
  // This is a lambda function that creates a DemoBase object
  // It creates a unique pointer to the object of the derived class
  using DemoCreator = std::function<std::unique_ptr<DemoBase>()>;
  // This is a pair of the lambda function and the name of the demo
  using DemoCreatorWithNameAndTags =
    std::tuple<DemoCreator, std::string, std::vector<DemoTag>>;

  static std::unique_ptr<DemoBase> createDemo(DemoType chosenDemo)
  {
    const auto demoMap = getDemoMap();
    const auto& it = demoMap.find(chosenDemo);
    if (it != demoMap.end())
    {
      return std::get<0>(it->second)(); // Call the associated lambda function
                                        // to create the object
    }
    return nullptr;
  }

  static const std::unordered_map<DemoType, DemoCreatorWithNameAndTags>&
  getDemoMap()
  {
    // Initialize the map statically
    static const std::unordered_map<DemoType, DemoCreatorWithNameAndTags>
      demoMap = {
        { DemoType::DeductingTypesDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<DeductingTypesDemo>(); },
            "DeductingTypesDemo",
            { DemoTag::Common }) },
        { DemoType::AutoDeductionDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<AutoDeductionDemo>(); },
            "AutoDeductionDemo",
            { DemoTag::Common }) },
        { DemoType::DeclareTypeDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<DeclareTypeDemo>(); },
            "DeclareTypeDemo",
            { DemoTag::Common }) },
        { DemoType::UndesiredTypesDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<UndesiredTypesDemo>(); },
            "UndesiredTypesDemo",
            { DemoTag::Common }) },
        { DemoType::AssigmentDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<AssigmentDemo>(); },
            "AssigmentDemo",
            { DemoTag::Common }) },
        { DemoType::OverloadingAndOverridingDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<OverloadingAndOverridingDemo>(); },
            "OverloadingAndOverridingDemo",
            { DemoTag::Common }) },
        { DemoType::PtrDemo,
          DemoCreatorWithNameAndTags([]()
                                     { return std::make_unique<PtrDemo>(); },
                                     "PtrDemo",
                                     { DemoTag::Common }) },
        { DemoType::ConstIteratorsDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<ConstIteratorsDemo>(); },
            "ConstIteratorsDemo",
            { DemoTag::Common }) },
        { DemoType::ThrowAndNoExceptDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<ThrowAndNoExceptDemo>(); },
            "ThrowAndNoExceptDemo",
            { DemoTag::Common }) },
        { DemoType::ConstExpressionDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<ConstExpressionDemo>(); },
            "ConstExpressionDemo",
            { DemoTag::Common }) },
        { DemoType::ConstMutableDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<ConstMutableDemo>(); },
            "ConstMutableDemo",
            { DemoTag::Common }) },
        { DemoType::RuleOfThreeDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<RuleOfThreeDemo>(); },
            "RuleOfThreeDemo",
            { DemoTag::Common }) },
        { DemoType::SOLID_Demo,
          DemoCreatorWithNameAndTags([]()
                                     { return std::make_unique<SOLID_Demo>(); },
                                     "SOLID_Demo",
                                     { DemoTag::Common }) },
        { DemoType::VirtualDestructorDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<VirtualDestructorDemo>(); },
            "VirtualDestructorDemo",
            { DemoTag::Common }) },
        { DemoType::ReferencePolymorphismDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<ReferencePolymorphismDemo>(); },
            "ReferencePolymorphismDemo",
            { DemoTag::Common }) },
        { DemoType::StaticMemoryAllocationDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<StaticMemoryAllocationDemo>(); },
            "StaticMemoryAllocationDemo",
            { DemoTag::Common }) },
        { DemoType::DeepShallowCopyDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<DeepShallowCopyDemo>(); },
            "DeepShallowCopyDemo",
            { DemoTag::Common }) },
        { DemoType::WeakPtrVsSharedPtrDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<WeakPtrVsSharedPtrDemo>(); },
            "WeakPtrVsSharedPtrDemo",
            { DemoTag::Common }) },
        { DemoType::ExceptionAndStackDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<ExceptionAndStackDemo>(); },
            "ExceptionAndStackDemo",
            { DemoTag::Common }) },
        { DemoType::CompilationProcessDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<CompilationProcessDemo>(); },
            "CompilationProcessDemo",
            { DemoTag::Common }) },
        { DemoType::UniquePtrFactoryDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<UniquePtrFactoryDemo>(); },
            "UniquePtrFactoryDemo",
            { DemoTag::Common }) },
        { DemoType::MoreAboutSharedPtrDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<MoreAboutSharedPtrDemo>(); },
            "MoreAboutSharedPtrDemo",
            { DemoTag::Common }) },
        { DemoType::HowToWeakPtrDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<HowToWeakPtrDemo>(); },
            "HowToWeakPtrDemo",
            { DemoTag::Common }) },
        { DemoType::AllocateSharedDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<AllocateSharedDemo>(); },
            "AllocateSharedDemo",
            { DemoTag::Common }) },
        { DemoType::PimplDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<PimplExample>(); },
            "PimplDemo",
            { DemoTag::DesignPatterns }) },
        { DemoType::UniversalRefVsRValueRefDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<UniversalRefVsRValueRefDemo>(); },
            "UniversalRefVsRValueRefDemo",
            { DemoTag::Common }) },
        { DemoType::ForwardDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<ForwardDemo>(); },
            "ForwardDemo",
            { DemoTag::Common }) },
        { DemoType::EmptyDemo,
          DemoCreatorWithNameAndTags([]()
                                     { return std::make_unique<EmptyDemo>(); },
                                     "EmptyDemo",
                                     { DemoTag::Common }) },
        { DemoType::AnyDemo,
          DemoCreatorWithNameAndTags([]()
                                     { return std::make_unique<AnyDemo>(); },
                                     "AnyDemo",
                                     { DemoTag::CPP17 }) },
        { DemoType::VariantDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<VariantDemo>(); },
            "VariantDemo",
            { DemoTag::CPP17 }) },
        { DemoType::StringViewDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<StringViewDemo>(); },
            "StringViewDemo",
            { DemoTag::CPP17 }) },
        { DemoType::CharConvDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<CharConvDemo>(); },
            "CharConvDemo",
            { DemoTag::CPP17 }) },
        { DemoType::ExecutionDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<ExecutionDemo>(); },
            "ExecutionDemo",
            { DemoTag::CPP17 }) },
        { DemoType::FileSystemDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<FileSystemDemo>(); },
            "FileSystemDemo",
            { DemoTag::CPP17 }) },
        { DemoType::MemoryResourceDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<MemoryResourceDemo>(); },
            "MemoryResourceDemo",
            { DemoTag::CPP17 }) },
        { DemoType::OptionalDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<OptionalDemo>(); },
            "OptionalDemo",
            { DemoTag::CPP17 }) },
        { DemoType::NeuralNetworkDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<NeuralNetworkDemo>(); },
            "NeuralNetworkDemo",
            { DemoTag::NeuralNetwork }) },
        { DemoType::SingletonDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<SingletonDemo>(); },
            "SingletonDemo",
            { DemoTag::DesignPatterns }) },
        { DemoType::BuilderDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<BuilderDemo>(); },
            "BuilderDemo",
            { DemoTag::DesignPatterns }) },
        { DemoType::AdapterDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<AdapterDemo>(); },
            "AdapterDemo",
            { DemoTag::DesignPatterns }) },
        { DemoType::BitDemo,
          DemoCreatorWithNameAndTags([]()
                                     { return std::make_unique<BitDemo>(); },
                                     "BitDemo",
                                     { DemoTag::CPP20 }) },
        { DemoType::SemaphoreDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<SemaphoreDemo>(); },
            "SemaphoreDemo",
            { DemoTag::CPP20 }) },
        { DemoType::CompareDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<CompareDemo>(); },
            "CompareDemo",
            { DemoTag::CPP20 }) },
        { DemoType::PredicateDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<PredicateDemo>(); },
            "PredicateDemo",
            { DemoTag::CPP20 }) },
        { DemoType::CorutineDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<CoroutineDemo>(); },
            "CoroutineDemo",
            { DemoTag::CPP20, DemoTag::DesignPatterns }) },
        { DemoType::HelloWorld2025Demo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<HelloWorld2025Demo>(); },
            "HelloWorld2025Demo",
            { DemoTag::GreboszCPP }) },
        { DemoType::TypeLimitsDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<TypeLimitsDemo>(); },
            "TypeLimitsDemo",
            { DemoTag::GreboszCPP }) },
        { DemoType::OutOfRangeDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<OutOfRangeDemo>(); },
            "OutOfRangeDemo",
            { DemoTag::Common }) },

        { DemoType::LongestCharSequenceDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<LongestCharSequence>(); },
            "LongestCharSequenceDemo",
            { DemoTag::QuestionTask }) },

        { DemoType::StringToIntoDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<StringToIntoDemo>(); },
            "StringToIntoDemo",
            { DemoTag::QuestionTask }) },
        { DemoType::LongestUniqueSubstringDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<LongestUniqueSubstring>(); },
            "LongestUniqueSubstringDemo",
            { DemoTag::QuestionTask }) },
        { DemoType::LongestPalindromDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<LongestPalindrom>(); },
            "LongestPalindromDemo",
            { DemoTag::QuestionTask }) },
        { DemoType::Zadanko4_16IIIDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<Zadanko4_16III>(); },
            "Zadanko4_16IIIDemo",
            { DemoTag::GreboszCPP }) },
        { DemoType::Zadanko8_28Demo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<Zadanko8_28>(); },
            "Zadanko8_28Demo",
            { DemoTag::GreboszCPP }) },
        { DemoType::Zadanko12_5_30Demo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<Zadanko12_5_30>(); },
            "Zadanko12_5_30Demo",
            { DemoTag::GreboszCPP }) },
           { DemoType::Zadanko13_9_10Demo,
            DemoCreatorWithNameAndTags(
            []() { return std::make_unique<Zadanko13_9_10>(); },
            "Zadanko13_9_10Demo",
            { DemoTag::GreboszCPP })},
            { DemoType::Zadanko14_7_16Demo,
            DemoCreatorWithNameAndTags(
            []() { return std::make_unique<Zadanko14_7_16>(); },
            "Zadanko14_7_16Demo",
            { DemoTag::GreboszCPP })},
        { DemoType::FutureObjDemo,
          DemoCreatorWithNameAndTags(
            []() { return std::make_unique<FutureObjDemo>(); },
            "FutureObjDemo",
            { DemoTag::CPP11 }) },

      };
    // std::cout << std::format("Demo map created. Size: {}.", sizeof(demoMap))
    //           << std::endl;
    return demoMap;
  }
};