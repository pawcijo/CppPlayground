#include "ElementReader.hpp"
#include "Element.hpp"

#include <cstddef>
#include <fstream>
#include <map>
#include <print>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

namespace PlaygroundLib
{
std::vector<Element> ElementReader::ReadElementsFromFile(
  const std::filesystem::path& filePath)
{

  std::vector<Element> elements;

  std::string getString{};
  std::ifstream file(filePath.string());
  if (!file)
    throw std::runtime_error("Cannot open file: " + filePath.string());

  bool firstLine = true;
  std::vector<std::string> names;
  while (std::getline(file, getString))
  {
    if (firstLine)
    {
      std::istringstream iss(getString);
      std::string value;
      while (std::getline(iss, value, ','))
      { // split by comma
        names.push_back(value);
      }
      // skip header line
      firstLine = false;
      continue;
    }

    std::istringstream iss(getString);
    std::string value;
    std::vector<std::string> parts;

    while (std::getline(iss, value, ','))
    { // split by comma
      parts.push_back(value);
    }

    int atomicNumber{};

    try
    {
      atomicNumber = std::stoi(parts[0]);
    }
    catch (std::invalid_argument& e)
    {

      std::printf("Invalid atomic number: %s\n", parts[0].c_str());
    }

    std::string name = parts[1];
    std::string symbol = parts[2];
    double atomicWeight{};

    try
    {
      atomicWeight = std::stod(parts[3]);
    }
    catch (std::invalid_argument& e)
    {

      std::printf("Invalid atomicWeight : %s\n", parts[3].c_str());
    }

    int neutrons{};

    try
    {
      neutrons = std::stoi(parts[4]);
    }
    catch (std::invalid_argument& e)
    {
      std::printf("Invalid number of neutrons : %s\n", parts[5].c_str());
    }

    int protons{};
    try
    {
      protons = std::stoi(parts[5]);
    }
    catch (std::invalid_argument& e)
    {
      std::printf("Invalid number of protons : %s\n", parts[4].c_str());
    }

    int electrons{};

    try
    {
      electrons = std::stoi(parts[6]);
    }
    catch (std::invalid_argument& e)
    {
      std::printf("Invalid number of electrons : %s\n", parts[6].c_str());
    }

    int period{};
    try
    {
      period = std::stoi(parts[7]);
    }
    catch (std::invalid_argument& e)
    {
      std::printf("Invalid number of period : %s\n", parts[7].c_str());
    }
    int group{};

    try
    {
      group = std::stoi(parts[8]);
    }
    catch (std::invalid_argument& e)
    {
      group = -1; // Unknown group
      std::printf("Unknown group : %s\n", parts[8].c_str());
    }

    std::string phaseAtSTP = parts[9];
    bool isRadioactive = parts[10].empty() ? false : true;

    std::map<std::string, std::string> additionalProperties;
    for (size_t i = 0; i < parts.size(); i++)
    {
      additionalProperties[names[i]] = parts[i]; // Placeholder logic
    }

    Element element(atomicNumber,
                    symbol,
                    name,
                    atomicWeight,
                    protons,
                    neutrons,
                    electrons,
                    period,
                    group,
                    phaseAtSTP,
                    isRadioactive,
                    additionalProperties);

    std::println("Read element: {}", element.toString());

    elements.push_back(element);
  }

  return elements;
}

}