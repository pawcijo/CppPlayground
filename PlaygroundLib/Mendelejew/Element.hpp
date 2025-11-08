#pragma once
#include <exception>
#include <ostream>
#include <print>
#include <sstream>
#include <string>
#include <unordered_map>
namespace PlaygroundLib
{

namespace
{

constexpr std::string kElementPropertyAtomicNumber = "Atomic Number";
constexpr std::string kElementPropertySymbol = "Symbol";
constexpr std::string kElementPropertyName = "Element";
constexpr std::string kElementPropertyAtomicWeight = "Atomic Mass";
constexpr std::string kElementPropertyPeriod = "Period";
constexpr std::string kElementPropertyGroup = "Group";
constexpr std::string kElementPropertyPhaseAtSTP = "Phase";
constexpr std::string kElementPropertyRadioactive = "Radioactive";
constexpr std::string kElementPropertyNumberOfProtons = "Number of Protons";
constexpr std::string kElementPropertyNumberOfNeutrons = "Number of Neutrons";
constexpr std::string kElementPropertyNumberOfElectrons = "Number of Electrons";

}

class Element
{
  std::unordered_map<std::string, std::string> mAllProperties;

public:
  explicit Element(std::unordered_map<std::string, std::string> properties = {})
    : mAllProperties(properties)
  {
  }

  int getAtomicNumber() const
  {
    int value{};
    try
    {
      value = std::stoi(mAllProperties.at(kElementPropertyAtomicNumber));
    }
    catch (std::exception& e)
    {
      std::println("{} exception, in {}.", e.what(), __func__);
    }
    return value;
  }
  const std::string& getSymbol() const
  {
    return mAllProperties.at(kElementPropertySymbol);
  }

  const std::string& getName() const
  {
    return mAllProperties.at(kElementPropertyName);
  }
  double getAtomicWeight() const
  {
    double mAtomicWeight{};
    try
    {
      mAtomicWeight =
        std::stod(mAllProperties.at(kElementPropertyAtomicWeight));
    }
    catch (std::exception& e)
    {
      std::println("{} exception, in {}.", e.what(), __func__);
    }
    return mAtomicWeight;
  }

  int getNumberOfProtons() const
  {
    int value{};
    try
    {
      value = std::stoi(mAllProperties.at(kElementPropertyNumberOfProtons));
    }
    catch (std::exception& e)
    {
      std::println("{} exception, in {}.", e.what(), __func__);
    }
    return value;
  }

  int getNumberOfNeutrons() const
  {
    int value{};
    try
    {
      value = std::stoi(mAllProperties.at(kElementPropertyNumberOfNeutrons));
    }
    catch (std::exception& e)
    {
      std::println("{} exception, in {}.", e.what(), __func__);
    }
    return value;
  }

  int getNumberOfElectrons() const
  {
    int value{};
    try
    {
      value = std::stoi(mAllProperties.at(kElementPropertyNumberOfElectrons));
    }
    catch (std::exception& e)
    {
      std::println("{} exception, in {}.", e.what(), __func__);
    }
    return value;
  }

  int getPeriod() const
  {
    int value{};
    try
    {
      value = std::stoi(mAllProperties.at(kElementPropertyPeriod));
    }
    catch (std::exception& e)
    {
      std::println("{} exception, in {}.", e.what(), __func__);
    }
    return value;
  }
  int getGroup() const
  {
    // return -1 if group is not defined
    int value{ -1 };

    if (mAllProperties.at(kElementPropertyGroup).empty())
    {
      return value;
    }

    try
    {
      value = std::stoi(mAllProperties.at(kElementPropertyGroup));
    }
    catch (std::exception& e)
    {
      std::println("{} exception, in {}.", e.what(), __func__);
    }
    return value;
  }
  const std::string& getPhaseAtSTP() const
  {
    return mAllProperties.at(kElementPropertyPhaseAtSTP);
    ;
  }
  bool isRadioactive() const
  {
    return mAllProperties.at(kElementPropertyRadioactive) == "yes" ? true
                                                                   : false;
  }

  const std::unordered_map<std::string, std::string>& getAllProperties() const
  {
    return mAllProperties;
  }

  std::string toString() const
  {
    std::ostringstream oss;
    oss << "Element: " << getName() << " (" << getSymbol()
        << "), Atomic Number: " << getAtomicNumber()
        << ", Atomic Weight: " << getAtomicWeight()
        << ", Protons: " << getNumberOfProtons()
        << ", Neutrons: " << getNumberOfNeutrons()
        << ", Electrons: " << getNumberOfElectrons()
        << ", Period: " << getPeriod() << ", Group: " << getGroup()
        << ", Phase at STP: " << getPhaseAtSTP()
        << ", Radioactive: " << (isRadioactive() ? "Yes" : "No");
    return oss.str();
  }
};

inline std::ostream& operator<<(std::ostream& os, const Element& elem)
{
  os << "Element: " << elem.getName() << " (" << elem.getSymbol()
     << "), Atomic Number: " << elem.getAtomicNumber()
     << ", Atomic Weight: " << elem.getAtomicWeight()
     << ", Protons: " << elem.getNumberOfProtons()
     << ", Neutrons: " << elem.getNumberOfNeutrons()
     << ", Electrons: " << elem.getNumberOfElectrons()
     << ", Period: " << elem.getPeriod() << ", Group: " << elem.getGroup()
     << ", Phase at STP: " << elem.getPhaseAtSTP()
     << ", Radioactive: " << (elem.isRadioactive() ? "Yes" : "No");
  return os;
}

}