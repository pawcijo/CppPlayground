#include "ElementReader.hpp"
#include "Element.hpp"

#include <cstddef>
#include <fstream>
#include <print>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
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
      while (std::getline(iss, value, ';'))
      {
        // split by comma
        names.push_back(value);
      }
      // skip header line
      firstLine = false;

      continue;
    }

    std::istringstream iss(getString);
    std::string value;
    std::vector<std::string> parts;
    while (std::getline(iss, value, ';'))
    { // split by comma

      parts.push_back(value);
    }
    std::unordered_map<std::string, std::string> additionalProperties;
    for (size_t i = 0; i < parts.size(); i++)
    {
      //std::println("{} {}", names[i], parts[i]);
      additionalProperties[names[i]] = parts[i]; // Placeholder logic
    }
    Element element(additionalProperties);
    // std::println("Read element: {}", element.toString());
    elements.push_back(element);
  }
  return elements;
}

}