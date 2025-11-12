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

    std::ifstream file(filePath.string());
    if (!file)
        throw std::runtime_error("Cannot open file: " + filePath.string());

    std::string line;
    bool firstLine = true;
    std::vector<std::string> names;

    // Lambda to trim whitespace from both ends
    auto trim = [](std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                        [](unsigned char c) { return !std::isspace(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(),
                             [](unsigned char c) { return !std::isspace(c); }).base(),
                s.end());
    };

    while (std::getline(file, line))
    {
        trim(line);
        if (line.empty())
            continue; // skip empty lines

        std::istringstream iss(line);
        std::string value;
        std::vector<std::string> parts;

        while (std::getline(iss, value, ','))
        {
            trim(value);
            parts.push_back(value);
        }

        if (firstLine)
        {
            // Header row
            names = parts;
            firstLine = false;
            continue;
        }

        if (parts.empty())
            continue; // skip empty data rows

        if (parts.size() != names.size())
        {
           // std::println("Warning: Row has {} columns but header has {}. Some data may be ignored.",
           //              parts.size(), names.size());
        }

        std::unordered_map<std::string, std::string> additionalProperties;
        size_t count = std::min(parts.size(), names.size());
        for (size_t i = 0; i < count; ++i)
        {
            additionalProperties[names[i]] = parts[i];
        }

        elements.emplace_back(additionalProperties);
    }

    return elements;
}

}