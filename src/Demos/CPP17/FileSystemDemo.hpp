#pragma once

#include <filesystem>  // Include for filesystem
#include <fstream>
#include <string>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

class FileSystemDemo : public DemoBase
{
   public:
    FileSystemDemo()
    {
        mName = "FileSystemDemo";
        mNotes = {
            {"std::filesystem",
             R"(The <filesystem> header in the C++ Standard Library provides facilities for performing 
operations on file systems and their components, such as paths, regular files, and directories.
It was introduced in C++17 and offers a convenient and standardized way to interact 
with the file system.)"}};
    }

    void PrintNotes() override
    {
        PrintName();
        for (const auto &pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample() override
    {
        PrintNotes();
        BasicExample();
        PermissionExample();
    }

    void ShowDemo() override
    {
        ShowExample();
    }

    void BasicExample()
    {
        LOG_FUNCTION_NAME(YELLOW);

        // Check if a path exists
        std::filesystem::path path = "resources/example_file.txt";
        if (std::filesystem::exists(path))
        {
            std::cout << "File exists: " << path << "\n";

            // Open file
            std::ifstream file(path);  // Open the file for reading
            if (!file)
            {
                std::cout << "Failed to open file: " << path << std::endl;
            }

            std::cout << GREEN_BOLD_TEXT_START << "File contents:\n" << TEXT_FORMAT_CLEAR;
            std::string line;
            while (std::getline(file, line))
            {
                std::cout << GREEN_TEXT_START << line << std::endl << TEXT_FORMAT_CLEAR;
            }

            file.close();  // Close the file
        }
        else
        {
            std::cout << "File does not exist: " << path << "\n";
        }

        // Check if a path is a regular file
        if (std::filesystem::is_regular_file(path))
        {
            std::cout << "File is a regular file: " << path << "\n";
        }
        else
        {
            std::cout << "File is not a regular file: " << path << "\n";
        }

        // Check if a path is a directory
        if (std::filesystem::is_directory(path))
        {
            std::cout << "File is a directory: " << path << "\n";
        }
        else
        {
            std::cout << "File is not a directory: " << path << "\n";
        }

        // Get the file size
        if (std::filesystem::exists(path))
        {
            std::cout << "File size: " << std::filesystem::file_size(path) << " bytes\n";
        }

        // Get the current path
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::cout << "Current path: " << currentPath << "\n";

        // Iterate over files in a directory
        std::filesystem::path directory = ".";
        for (const auto &entry : std::filesystem::directory_iterator(directory))
        {
            std::cout << entry.path() << "\n";
        }

        // Create a directory
        std::filesystem::path newDirectory = "new_directory";
        if (std::filesystem::create_directory(newDirectory))
        {
            std::cout << BLUE_TEXT_START << "Directory created: " << newDirectory << "\n";
        }
        else
        {
            std::cout << "Directory not created: " << newDirectory << "\n";
        }

        // Remove a directory
        if (std::filesystem::remove(newDirectory))
        {
            std::cout << RED_TEXT_START << "Directory removed: " << newDirectory << "\n";
        }
        else
        {
            std::cout << "Directory not removed: " << newDirectory << "\n";
        }
    }

    void PermissionExample()
    {
        LOG_FUNCTION_NAME(YELLOW);

        std::filesystem::path filePath = "resources/example_file.txt";
        checkPermissions(filePath);
    }

    void checkPermissions(const std::filesystem::path &filePath)
    {
        namespace fs = std::filesystem;
        if (!fs::exists(filePath))
        {
            std::cerr << "File does not exist: " << filePath << std::endl;
            return;
        }

        fs::perms p = fs::status(filePath).permissions();

        std::cout << "Permissions for " << filePath << ":\n";
        std::cout << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-");
        std::cout << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-");
        std::cout << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-");
        std::cout << " (Owner)\n";

        std::cout << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-");
        std::cout << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-");
        std::cout << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-");
        std::cout << " (Group)\n";

        std::cout << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-");
        std::cout << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-");
        std::cout << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
        std::cout << " (Others)\n";
    }
};