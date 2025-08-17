#pragma once

#include "VulkanPipeline.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "temporary.hpp"
#include <vector>
#include <set>
#include <string>
#include <vulkan/vulkan_core.h>
#include <stdexcept>
	
#include <algorithm>

const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"
};

#ifdef __APPLE__
const std::vector<const char*> deviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  "VK_KHR_portability_subset"
};
#else
const std::vector<const char*> deviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
#endif

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class Utils
{
public:
  static std::vector<char> readFile(const std::string& filename);

  static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                                       VkSurfaceKHR surface)
  {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
      device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(
        device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
      device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
  }

  static bool checkDeviceExtensionSupport(VkPhysicalDevice device,
                                          VkSurfaceKHR surface)
  {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(
      device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
      device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                             deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                              VkSurfaceKHR surface)
  {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        indices.graphicsFamily = i;
      }

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

      if (presentSupport)
      {
        indices.presentFamily = i;
      }

      if (indices.isComplete())
      {
        break;
      }

      i++;
    }

    return indices;
  }

  static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
  {
    QueueFamilyIndices indices = findQueueFamilies(device, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device, surface);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
      SwapChainSupportDetails swapChainSupport =
        querySwapChainSupport(device, surface);
      swapChainAdequate = !swapChainSupport.formats.empty() &&
                          !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate &&
           supportedFeatures.samplerAnisotropy;
  }

  static VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats)
  {
    for (const auto& availableFormat : availableFormats)
    {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        return availableFormat;
      }
    }

    return availableFormats[0];
  }

  static VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes)
  {
    for (const auto& availablePresentMode : availablePresentModes)
    {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
      {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  static VkExtent2D chooseSwapExtent(
    const VkSurfaceCapabilitiesKHR& capabilities,
    GLFWwindow* window)
  {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max())
    {
      return capabilities.currentExtent;
    }
    else
    {
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);

      VkExtent2D actualExtent = { static_cast<uint32_t>(width),
                                  static_cast<uint32_t>(height) };

      actualExtent.width = std::clamp(actualExtent.width,
                                      capabilities.minImageExtent.width,
                                      capabilities.maxImageExtent.width);
      actualExtent.height = std::clamp(actualExtent.height,
                                       capabilities.minImageExtent.height,
                                       capabilities.maxImageExtent.height);

      return actualExtent;
    }
  }

  static VkShaderModule createShaderModule(const std::vector<char>& code,
                                           VkDevice device)
  {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) !=
        VK_SUCCESS)
    {
      throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
  }

  static uint32_t findMemoryType(uint32_t typeFilter,
                                 VkMemoryPropertyFlags properties,
                                 VkPhysicalDevice physicalDevice)
  {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
      if ((typeFilter & (1 << i)) &&
          (memProperties.memoryTypes[i].propertyFlags & properties) ==
            properties)
      {
        return i;
      }
    }

    throw std::runtime_error("failed to find suitable memory type!");
  }

  static void createBuffer(VkDeviceSize size,
                           VkBufferUsageFlags usage,
                           VkMemoryPropertyFlags properties,
                           VkBuffer& buffer,
                           VkDeviceMemory& bufferMemory,
                           VkDevice device,
                           VkPhysicalDevice physiacalDevice)
  {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
      memRequirements.memoryTypeBits, properties, physiacalDevice);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) !=
        VK_SUCCESS)
    {
      throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
  }

  static void createImage(uint32_t width,
                          uint32_t height,
                          VkFormat format,
                          VkImageTiling tiling,
                          VkImageUsageFlags usage,
                          VkMemoryPropertyFlags properties,
                          VkImage& image,
                          VkDeviceMemory& imageMemory,
                          VkDevice device,
                          VkPhysicalDevice physicalDevice)
  {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
      memRequirements.memoryTypeBits, properties, physicalDevice);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) !=
        VK_SUCCESS)
    {
      throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device, image, imageMemory, 0);
  }

  static VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool,
                                                 VkDevice device)
  {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
  }

  static void endSingleTimeCommands(VkCommandBuffer commandBuffer,
                                    VkQueue graphicsQueue,
                                    VkCommandPool commandPool,
                                    VkDevice device)
  {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
  }

  static void transitionImageLayout(VkImage image,
                                    VkFormat format,
                                    VkImageLayout oldLayout,
                                    VkImageLayout newLayout,
                                    VkCommandPool commandPool,
                                    VkDevice device,
                                    VkQueue graphicsQueue)
  {
    VkCommandBuffer commandBuffer =
      beginSingleTimeCommands(commandPool, device);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
      throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(commandBuffer,
                         sourceStage,
                         destinationStage,
                         0,
                         0,
                         nullptr,
                         0,
                         nullptr,
                         1,
                         &barrier);

    endSingleTimeCommands(commandBuffer, graphicsQueue, commandPool, device);
  }

  static void copyBufferToImage(VkBuffer buffer,
                                VkImage image,
                                uint32_t width,
                                uint32_t height,
                                VkCommandPool commandPool,
                                VkDevice device,
                                VkQueue graphicsQueue)
  {
    VkCommandBuffer commandBuffer =
      beginSingleTimeCommands(commandPool, device);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(commandBuffer,
                           buffer,
                           image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,
                           &region);

    endSingleTimeCommands(commandBuffer, graphicsQueue, commandPool, device);
  }

  static void copyBuffer(VkBuffer srcBuffer,
                         VkBuffer dstBuffer,
                         VkDeviceSize size,
                         VkCommandPool commandPool,
                         VkDevice device,
                         VkQueue graphicsQueue)
  {
    VkCommandBuffer commandBuffer =
      beginSingleTimeCommands(commandPool, device);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer, graphicsQueue, commandPool, device);
  }
};
