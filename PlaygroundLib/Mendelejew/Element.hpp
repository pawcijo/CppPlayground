#pragma once
#include <map>
#include <ostream>
#include <string>
#include <sstream>
namespace PlaygroundLib
{

class Element
{
  int mAtomicNumber;
  std::string mSymbol;
  std::string mName;
  double mAtomicWeight;

  int numberOfProtons;
  int numberOfNeutrons;
  int numberOfElectrons;
  int mPeroid;
  int mGroup;
  std::string mPhaseAtSTP;
  bool mIsRadioactive;

  std::map<std::string, std::string> mAllProperties;

public:
  explicit Element(int atomicNumber,
                   std::string symbol,
                   std::string name,
                   double atomicWeight,
                   int protons,
                   int neutrons,
                   int electrons,
                   int period,
                   int group,
                   std::string phaseAtSTP,
                   bool isRadioactive,
                   std::map<std::string, std::string> properties = {})
    : mAtomicNumber(atomicNumber)
    , mSymbol(symbol)
    , mName(name)
    , mAtomicWeight(atomicWeight)
    , numberOfProtons(protons)
    , numberOfNeutrons(neutrons)
    , numberOfElectrons(electrons)
    , mPeroid(period)
    , mGroup(group)
    , mPhaseAtSTP(phaseAtSTP)
    , mIsRadioactive(isRadioactive)
    , mAllProperties(properties)
  {
  }

  int getAtomicNumber() const
  {
    return mAtomicNumber;
  }
  const std::string& getSymbol() const
  {
    return mSymbol;
  }
  const std::string& getName() const
  {
    return mName;
  }
  double getAtomicWeight() const
  {
    return mAtomicWeight;
  }

  int getNumberOfProtons() const
  {
    return numberOfProtons;
  }

  int getNumberOfNeutrons() const
  {
    return numberOfNeutrons;
  }

  int getNumberOfElectrons() const
  {
    return numberOfElectrons;
  }

  int getPeriod() const
  {
    return mPeroid;
  }
  int getGroup() const
  {
    return mGroup;
  }
  const std::string& getPhaseAtSTP() const
  {
    return mPhaseAtSTP;
  }
  bool isRadioactive() const
  {
    return mIsRadioactive;
  }

  const std::map<std::string, std::string>& getAllProperties() const
  {
    return mAllProperties;
  }

  std::string toString() const
  {
    std::ostringstream oss;
    oss << "Element: " << mName << " (" << mSymbol
        << "), Atomic Number: " << mAtomicNumber
        << ", Atomic Weight: " << mAtomicWeight
        << ", Protons: " << numberOfProtons
        << ", Neutrons: " << numberOfNeutrons
        << ", Electrons: " << numberOfElectrons << ", Period: " << mPeroid
        << ", Group: " << mGroup << ", Phase at STP: " << mPhaseAtSTP
        << ", Radioactive: " << (mIsRadioactive ? "Yes" : "No");
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