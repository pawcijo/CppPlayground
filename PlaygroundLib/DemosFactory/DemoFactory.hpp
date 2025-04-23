#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>

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
#include "Demos/Patterns/Builder/BuilderDemo.hpp"
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

// Design patterns
#include "Demos/Patterns/Pimpl.hpp"
#include "Demos/Patterns/Singleton/SingletonDemo.hpp"
#include "Demos/Patterns/Builder/BuilderDemo.hpp"
#include "Demos/Patterns/Adapter/AdapterDemo.hpp"

// C++ 17
#include "Demos/CPP17/AnyDemo.hpp"
#include "Demos/CPP17/CharConvDemo.hpp"
#include "Demos/CPP17/ExecutionDemo.hpp"
#include "Demos/CPP17/FileSystemDemo.hpp"
#include "Demos/CPP17/MemoryResourceDemo.hpp"
#include "Demos/CPP17/OptionalDemo.hpp"
#include "Demos/CPP17/StringViewDemo.hpp"
#include "Demos/CPP17/VariantDemo.hpp"

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
  Count
};

 // This is a lambda function that creates a DemoBase object
  // It creates a unique pointer to the object of the derived class
  using DemoCreator = std::function<std::unique_ptr<DemoBase>()>;
  // This is a pair of the lambda function and the name of the demo
  using DemoCreatorWithName = std::pair<DemoCreator, std::string>;

#define ENUM_TO_STRING(name) #name
/*
This is also design pattern. :)
Abstract factory as BaseDemo is form of abstraction.
*/
class DemoFactory
{
public:
 

  static std::unique_ptr<DemoBase> createDemo(DemoType chosenDemo)
  {
    const auto& it = getDemoMap().find(chosenDemo);
    if (it != getDemoMap().end())
    {
      return it->second
        .first(); // Call the associated lambda function to create the object
    }
    return nullptr;
  }

  static auto getDemoMap() -> std::map<DemoType, DemoCreatorWithName>&
  {
    // Initialize the map statically
    static std::map<DemoType, DemoCreatorWithName> demoMap = {
      { DemoType::DeductingTypesDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<DeductingTypesDemo>(); },
                            "DeductingTypesDemo") },
      { DemoType::AutoDeductionDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<AutoDeductionDemo>(); },
                            "AutoDeductionDemo") },
      { DemoType::DeclareTypeDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<DeclareTypeDemo>(); },
                            "DeclareTypeDemo") },
      { DemoType::UndesiredTypesDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<UndesiredTypesDemo>(); },
                            "UndesiredTypesDemo") },
      { DemoType::AssigmentDemo,
        DemoCreatorWithName([]() { return std::make_unique<AssigmentDemo>(); },
                            "AssigmentDemo") },
      { DemoType::OverloadingAndOverridingDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<OverloadingAndOverridingDemo>(); },
          "OverloadingAndOverridingDemo") },
      { DemoType::PtrDemo,
        DemoCreatorWithName([]() { return std::make_unique<PtrDemo>(); },
                            "PtrDemo") },
      { DemoType::ConstIteratorsDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<ConstIteratorsDemo>(); },
                            "ConstIteratorsDemo") },
      { DemoType::ThrowAndNoExceptDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<ThrowAndNoExceptDemo>(); },
          "ThrowAndNoExceptDemo") },
      { DemoType::ConstExpressionDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<ConstExpressionDemo>(); },
                            "ConstExpressionDemo") },
      { DemoType::ConstMutableDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<ConstMutableDemo>(); },
                            "ConstMutableDemo") },
      { DemoType::RuleOfThreeDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<RuleOfThreeDemo>(); },
                            "RuleOfThreeDemo") },
      { DemoType::SOLID_Demo,
        DemoCreatorWithName([]() { return std::make_unique<SOLID_Demo>(); },
                            "SOLID_Demo") },
      { DemoType::VirtualDestructorDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<VirtualDestructorDemo>(); },
          "VirtualDestructorDemo") },
      { DemoType::ReferencePolymorphismDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<ReferencePolymorphismDemo>(); },
          "ReferencePolymorphismDemo") },
      { DemoType::StaticMemoryAllocationDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<StaticMemoryAllocationDemo>(); },
          "StaticMemoryAllocationDemo") },
      { DemoType::DeepShallowCopyDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<DeepShallowCopyDemo>(); },
                            "DeepShallowCopyDemo") },
      { DemoType::WeakPtrVsSharedPtrDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<WeakPtrVsSharedPtrDemo>(); },
          "WeakPtrVsSharedPtrDemo") },
      { DemoType::ExceptionAndStackDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<ExceptionAndStackDemo>(); },
          "ExceptionAndStackDemo") },
      { DemoType::CompilationProcessDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<CompilationProcessDemo>(); },
          "CompilationProcessDemo") },
      { DemoType::UniquePtrFactoryDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<UniquePtrFactoryDemo>(); },
          "UniquePtrFactoryDemo") },
      { DemoType::MoreAboutSharedPtrDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<MoreAboutSharedPtrDemo>(); },
          "MoreAboutSharedPtrDemo") },
      { DemoType::HowToWeakPtrDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<HowToWeakPtrDemo>(); },
                            "HowToWeakPtrDemo") },
      { DemoType::AllocateSharedDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<AllocateSharedDemo>(); },
                            "AllocateSharedDemo") },
      { DemoType::PimplDemo,
        DemoCreatorWithName([]() { return std::make_unique<PimplExample>(); },
                            "PimplDemo") },
      { DemoType::UniversalRefVsRValueRefDemo,
        DemoCreatorWithName(
          []() { return std::make_unique<UniversalRefVsRValueRefDemo>(); },
          "UniversalRefVsRValueRefDemo") },
      { DemoType::ForwardDemo,
        DemoCreatorWithName([]() { return std::make_unique<ForwardDemo>(); },
                            "ForwardDemo") },
      { DemoType::EmptyDemo,
        DemoCreatorWithName([]() { return std::make_unique<EmptyDemo>(); },
                            "EmptyDemo") },
      { DemoType::AnyDemo,
        DemoCreatorWithName([]() { return std::make_unique<AnyDemo>(); },
                            "AnyDemo") },
      { DemoType::VariantDemo,
        DemoCreatorWithName([]() { return std::make_unique<VariantDemo>(); },
                            "VariantDemo") },
      { DemoType::StringViewDemo,
        DemoCreatorWithName([]() { return std::make_unique<StringViewDemo>(); },
                            "StringViewDemo") },
      { DemoType::CharConvDemo,
        DemoCreatorWithName([]() { return std::make_unique<CharConvDemo>(); },
                            "CharConvDemo") },
      { DemoType::ExecutionDemo,
        DemoCreatorWithName([]() { return std::make_unique<ExecutionDemo>(); },
                            "ExecutionDemo") },
      { DemoType::FileSystemDemo,
        DemoCreatorWithName([]() { return std::make_unique<FileSystemDemo>(); },
                            "FileSystemDemo") },
      { DemoType::MemoryResourceDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<MemoryResourceDemo>(); },
                            "MemoryResourceDemo") },
      { DemoType::OptionalDemo,
        DemoCreatorWithName([]() { return std::make_unique<OptionalDemo>(); },
                            "OptionalDemo") },
      { DemoType::NeuralNetworkDemo,
        DemoCreatorWithName([]()
                            { return std::make_unique<NeuralNetworkDemo>(); },
                            "NeuralNetworkDemo") },
      { DemoType::SingletonDemo,
        DemoCreatorWithName([]() { return std::make_unique<SingletonDemo>(); },
                            "SingletonDemo") },
      { DemoType::BuilderDemo,
        DemoCreatorWithName([]() { return std::make_unique<BuilderDemo>(); },
                            "BuilderDemo") },
      { DemoType::AdapterDemo,
        DemoCreatorWithName([]() { return std::make_unique<AdapterDemo>(); },
                            "AdapterDemo") }
    };
    return demoMap;
  }
};