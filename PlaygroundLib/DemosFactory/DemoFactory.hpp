#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "CommonDemos.h" // IWYU pragma: keep

// Design patterns
#include "Demos/Patterns/Adapter/AdapterDemo.hpp"
#include "Demos/Patterns/Builder/BuilderDemo.hpp"
#include "Demos/Patterns/Pimpl.hpp"
#include "Demos/Patterns/Singleton/SingletonDemo.hpp"

// C++ 11
#include "Demos/CPP11/AtomicDemo.hpp"
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
#include "Demos/CPP20/CoroutineDemo.hpp"
#include "Demos/CPP20/NumbersDemo.hpp"
#include "Demos/CPP20/PredicateDemo.hpp"
#include "Demos/CPP20/RangesDemo.hpp"
#include "Demos/CPP20/SemaphoreDemo.hpp"
#include "Demos/CPP20/ViewDemo.hpp"
#include "Demos/CPP20/FormatDemo.hpp"

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
#include "QuestionTasks/DecodeString.hpp"

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
  NumbersDemo,
  RangesDemo,
  PredicateDemo,
  CoroutineDemo,
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
  AtomicDemo,
  FutureObjDemo,
  ViewDemo,
  FormatDemo,
  DecodeStringDemo,
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

  struct DemoInfo
  {
    DemoCreator creator;
    std::string name;
    std::vector<DemoTag> tags;

    DemoInfo(DemoCreator creator,
             std::string name,
             std::vector<DemoTag> tags)
      : creator(std::move(creator))
      , name(std::move(name))
      , tags(std::move(tags))
    {
    }
  };

  static std::unique_ptr<DemoBase> createDemo(DemoType chosenDemo)
  {
    const auto demoMap = getDemoMap();
    const auto& it = demoMap.find(chosenDemo);
    if (it != demoMap.end())
    {
      return it->second.creator();
    }
    return nullptr;
  }

  static const std::unordered_map<DemoType, DemoInfo>&
  getDemoMap()
  {
    // Initialize the map statically
    static const std::unordered_map<DemoType, DemoInfo>
      demoMap = {
        { DemoType::DeductingTypesDemo,
          DemoInfo(
            []() { return std::make_unique<DeductingTypesDemo>(); },
            "DeductingTypesDemo",
            { DemoTag::Common }) },
        { DemoType::AutoDeductionDemo,
          DemoInfo(
            []() { return std::make_unique<AutoDeductionDemo>(); },
            "AutoDeductionDemo",
            { DemoTag::Common }) },
        { DemoType::DeclareTypeDemo,
          DemoInfo(
            []() { return std::make_unique<DeclareTypeDemo>(); },
            "DeclareTypeDemo",
            { DemoTag::Common }) },
        { DemoType::UndesiredTypesDemo,
          DemoInfo(
            []() { return std::make_unique<UndesiredTypesDemo>(); },
            "UndesiredTypesDemo",
            { DemoTag::Common }) },
        { DemoType::AssigmentDemo,
          DemoInfo(
            []() { return std::make_unique<AssigmentDemo>(); },
            "AssigmentDemo",
            { DemoTag::Common }) },
        { DemoType::OverloadingAndOverridingDemo,
          DemoInfo(
            []() { return std::make_unique<OverloadingAndOverridingDemo>(); },
            "OverloadingAndOverridingDemo",
            { DemoTag::Common }) },
        { DemoType::PtrDemo,
          DemoInfo([]()
                                     { return std::make_unique<PtrDemo>(); },
                                     "PtrDemo",
                                     { DemoTag::Common }) },
        { DemoType::ConstIteratorsDemo,
          DemoInfo(
            []() { return std::make_unique<ConstIteratorsDemo>(); },
            "ConstIteratorsDemo",
            { DemoTag::Common }) },
        { DemoType::ThrowAndNoExceptDemo,
          DemoInfo(
            []() { return std::make_unique<ThrowAndNoExceptDemo>(); },
            "ThrowAndNoExceptDemo",
            { DemoTag::Common }) },
        { DemoType::ConstExpressionDemo,
          DemoInfo(
            []() { return std::make_unique<ConstExpressionDemo>(); },
            "ConstExpressionDemo",
            { DemoTag::Common }) },
        { DemoType::ConstMutableDemo,
          DemoInfo(
            []() { return std::make_unique<ConstMutableDemo>(); },
            "ConstMutableDemo",
            { DemoTag::Common }) },
        { DemoType::RuleOfThreeDemo,
          DemoInfo(
            []() { return std::make_unique<RuleOfThreeDemo>(); },
            "RuleOfThreeDemo",
            { DemoTag::Common }) },
        { DemoType::SOLID_Demo,
          DemoInfo([]()
                                     { return std::make_unique<SOLID_Demo>(); },
                                     "SOLID_Demo",
                                     { DemoTag::Common }) },
        { DemoType::VirtualDestructorDemo,
          DemoInfo(
            []() { return std::make_unique<VirtualDestructorDemo>(); },
            "VirtualDestructorDemo",
            { DemoTag::Common }) },
        { DemoType::ReferencePolymorphismDemo,
          DemoInfo(
            []() { return std::make_unique<ReferencePolymorphismDemo>(); },
            "ReferencePolymorphismDemo",
            { DemoTag::Common }) },
        { DemoType::StaticMemoryAllocationDemo,
          DemoInfo(
            []() { return std::make_unique<StaticMemoryAllocationDemo>(); },
            "StaticMemoryAllocationDemo",
            { DemoTag::Common }) },
        { DemoType::DeepShallowCopyDemo,
          DemoInfo(
            []() { return std::make_unique<DeepShallowCopyDemo>(); },
            "DeepShallowCopyDemo",
            { DemoTag::Common }) },
        { DemoType::WeakPtrVsSharedPtrDemo,
          DemoInfo(
            []() { return std::make_unique<WeakPtrVsSharedPtrDemo>(); },
            "WeakPtrVsSharedPtrDemo",
            { DemoTag::Common }) },
        { DemoType::ExceptionAndStackDemo,
          DemoInfo(
            []() { return std::make_unique<ExceptionAndStackDemo>(); },
            "ExceptionAndStackDemo",
            { DemoTag::Common }) },
        { DemoType::CompilationProcessDemo,
          DemoInfo(
            []() { return std::make_unique<CompilationProcessDemo>(); },
            "CompilationProcessDemo",
            { DemoTag::Common }) },
        { DemoType::UniquePtrFactoryDemo,
          DemoInfo(
            []() { return std::make_unique<UniquePtrFactoryDemo>(); },
            "UniquePtrFactoryDemo",
            { DemoTag::Common }) },
        { DemoType::MoreAboutSharedPtrDemo,
          DemoInfo(
            []() { return std::make_unique<MoreAboutSharedPtrDemo>(); },
            "MoreAboutSharedPtrDemo",
            { DemoTag::Common }) },
        { DemoType::HowToWeakPtrDemo,
          DemoInfo(
            []() { return std::make_unique<HowToWeakPtrDemo>(); },
            "HowToWeakPtrDemo",
            { DemoTag::Common }) },
        { DemoType::AllocateSharedDemo,
          DemoInfo(
            []() { return std::make_unique<AllocateSharedDemo>(); },
            "AllocateSharedDemo",
            { DemoTag::Common }) },
        { DemoType::PimplDemo,
          DemoInfo(
            []() { return std::make_unique<PimplExample>(); },
            "PimplDemo",
            { DemoTag::DesignPatterns }) },
        { DemoType::UniversalRefVsRValueRefDemo,
          DemoInfo(
            []() { return std::make_unique<UniversalRefVsRValueRefDemo>(); },
            "UniversalRefVsRValueRefDemo",
            { DemoTag::Common }) },
        { DemoType::ForwardDemo,
          DemoInfo(
            []() { return std::make_unique<ForwardDemo>(); },
            "ForwardDemo",
            { DemoTag::Common }) },
        { DemoType::EmptyDemo,
          DemoInfo([]()
                                     { return std::make_unique<EmptyDemo>(); },
                                     "EmptyDemo",
                                     { DemoTag::Common }) },
        { DemoType::AnyDemo,
          DemoInfo([]()
                                     { return std::make_unique<AnyDemo>(); },
                                     "AnyDemo",
                                     { DemoTag::CPP17 }) },
        { DemoType::VariantDemo,
          DemoInfo(
            []() { return std::make_unique<VariantDemo>(); },
            "VariantDemo",
            { DemoTag::CPP17 }) },
        { DemoType::StringViewDemo,
          DemoInfo(
            []() { return std::make_unique<StringViewDemo>(); },
            "StringViewDemo",
            { DemoTag::CPP17 }) },
        { DemoType::CharConvDemo,
          DemoInfo(
            []() { return std::make_unique<CharConvDemo>(); },
            "CharConvDemo",
            { DemoTag::CPP17 }) },
        { DemoType::ExecutionDemo,
          DemoInfo(
            []() { return std::make_unique<ExecutionDemo>(); },
            "ExecutionDemo",
            { DemoTag::CPP17 }) },
        { DemoType::FileSystemDemo,
          DemoInfo(
            []() { return std::make_unique<FileSystemDemo>(); },
            "FileSystemDemo",
            { DemoTag::CPP17 }) },
        { DemoType::MemoryResourceDemo,
          DemoInfo(
            []() { return std::make_unique<MemoryResourceDemo>(); },
            "MemoryResourceDemo",
            { DemoTag::CPP17 }) },
        { DemoType::OptionalDemo,
          DemoInfo(
            []() { return std::make_unique<OptionalDemo>(); },
            "OptionalDemo",
            { DemoTag::CPP17 }) },
        { DemoType::NeuralNetworkDemo,
          DemoInfo(
            []() { return std::make_unique<NeuralNetworkDemo>(); },
            "NeuralNetworkDemo",
            { DemoTag::NeuralNetwork }) },
        { DemoType::SingletonDemo,
          DemoInfo(
            []() { return std::make_unique<SingletonDemo>(); },
            "SingletonDemo",
            { DemoTag::DesignPatterns }) },
        { DemoType::BuilderDemo,
          DemoInfo(
            []() { return std::make_unique<BuilderDemo>(); },
            "BuilderDemo",
            { DemoTag::DesignPatterns }) },
        { DemoType::AdapterDemo,
          DemoInfo(
            []() { return std::make_unique<AdapterDemo>(); },
            "AdapterDemo",
            { DemoTag::DesignPatterns }) },
        { DemoType::BitDemo,
          DemoInfo([]()
                                     { return std::make_unique<BitDemo>(); },
                                     "BitDemo",
                                     { DemoTag::CPP20 }) },
        { DemoType::SemaphoreDemo,
          DemoInfo(
            []() { return std::make_unique<SemaphoreDemo>(); },
            "SemaphoreDemo",
            { DemoTag::CPP20 }) },
        { DemoType::CompareDemo,
          DemoInfo(
            []() { return std::make_unique<CompareDemo>(); },
            "CompareDemo",
            { DemoTag::CPP20 }) },
        { DemoType::NumbersDemo,
          DemoInfo(
            []() { return std::make_unique<NumbersDemo>(); },
            "NumbersDemo",
            { DemoTag::CPP20 }) },
        { DemoType::RangesDemo,
          DemoInfo(
            []() { return std::make_unique<RangesDemo>(); },
            "RangesDemo",
            { DemoTag::CPP20 }) },
        { DemoType::PredicateDemo,
          DemoInfo(
            []() { return std::make_unique<PredicateDemo>(); },
            "PredicateDemo",
            { DemoTag::CPP20 }) },
        { DemoType::CoroutineDemo,
          DemoInfo(
            []() { return std::make_unique<CoroutineDemo>(); },
            "CoroutineDemo",
            { DemoTag::CPP20, DemoTag::DesignPatterns }) },
        { DemoType::HelloWorld2025Demo,
          DemoInfo(
            []() { return std::make_unique<HelloWorld2025Demo>(); },
            "HelloWorld2025Demo",
            { DemoTag::GreboszCPP }) },
        { DemoType::TypeLimitsDemo,
          DemoInfo(
            []() { return std::make_unique<TypeLimitsDemo>(); },
            "TypeLimitsDemo",
            { DemoTag::GreboszCPP }) },
        { DemoType::OutOfRangeDemo,
          DemoInfo(
            []() { return std::make_unique<OutOfRangeDemo>(); },
            "OutOfRangeDemo",
            { DemoTag::Common }) },

        { DemoType::LongestCharSequenceDemo,
          DemoInfo(
            []() { return std::make_unique<LongestCharSequence>(); },
            "LongestCharSequenceDemo",
            { DemoTag::QuestionTask }) },

        { DemoType::StringToIntoDemo,
          DemoInfo(
            []() { return std::make_unique<StringToIntoDemo>(); },
            "StringToIntoDemo",
            { DemoTag::QuestionTask }) },
        { DemoType::LongestUniqueSubstringDemo,
          DemoInfo(
            []() { return std::make_unique<LongestUniqueSubstring>(); },
            "LongestUniqueSubstringDemo",
            { DemoTag::QuestionTask }) },
        { DemoType::LongestPalindromDemo,
          DemoInfo(
            []() { return std::make_unique<LongestPalindrom>(); },
            "LongestPalindromDemo",
            { DemoTag::QuestionTask }) },
        { DemoType::Zadanko4_16IIIDemo,
          DemoInfo(
            []() { return std::make_unique<Zadanko4_16III>(); },
            "Zadanko4_16IIIDemo",
            { DemoTag::GreboszCPP }) },
        { DemoType::Zadanko8_28Demo,
          DemoInfo(
            []() { return std::make_unique<Zadanko8_28>(); },
            "Zadanko8_28Demo",
            { DemoTag::GreboszCPP }) },
        { DemoType::Zadanko12_5_30Demo,
          DemoInfo(
            []() { return std::make_unique<Zadanko12_5_30>(); },
            "Zadanko12_5_30Demo",
            { DemoTag::GreboszCPP }) },
           { DemoType::Zadanko13_9_10Demo,
            DemoInfo(
            []() { return std::make_unique<Zadanko13_9_10>(); },
            "Zadanko13_9_10Demo",
            { DemoTag::GreboszCPP })},
            { DemoType::Zadanko14_7_16Demo,
            DemoInfo(
            []() { return std::make_unique<Zadanko14_7_16>(); },
            "Zadanko14_7_16Demo",
            { DemoTag::GreboszCPP })},
        { DemoType::AtomicDemo,
          DemoInfo(
            []() { return std::make_unique<AtomicDemo>(); },
            "AtomicDemo",
            { DemoTag::CPP11 }) },
        { DemoType::FutureObjDemo,
          DemoInfo(
            []() { return std::make_unique<FutureObjDemo>(); },
            "FutureObjDemo",
            { DemoTag::CPP11 }) },
        { DemoType::ViewDemo,
          DemoInfo(
            []() { return std::make_unique<ViewDemo>(); },
            "ViewDemo",
            { DemoTag::CPP20 }) },
        { DemoType::FormatDemo,
          DemoInfo(
            []() { return std::make_unique<FormatDemo>(); },
            "FormatDemo",
            { DemoTag::CPP20 }) },
            
          { DemoType::DecodeStringDemo,
          DemoInfo(
            []() { return std::make_unique<DecodeStringDemo>(); },
            "DecodeStringDemo",
            { DemoTag::QuestionTask }) },
      };
    // std::cout << std::format("Demo map created. Size: {}.", sizeof(demoMap))
    //           << std::endl;
    return demoMap;
  }
};