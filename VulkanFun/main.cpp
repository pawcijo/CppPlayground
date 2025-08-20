#include "Vulkan/VulkanPipeLine.hpp"
#include "Vulkan/VulkanApp.hpp"

#include <stdexcept>
#include <iostream>


int main()
{
    GPGVulkan::VulkanPipeLine appWindow(1920, 1080);
    GPGVulkan::VulkanApp app(appWindow);

    try
    {
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cout<<"Exception: " <<e.what()<< std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

    return 0;
}
