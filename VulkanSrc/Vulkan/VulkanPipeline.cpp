#include "VulkanPipeline.hpp"
#include "PlaygroundLib/Common/DemoBase.hpp"
#include "external/ImGui/imgui.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define _GLFW_VULKAN_LIBRARY

#include "VulkanApp.hpp"
#include "VulkanShader.hpp"
#include "VulkanValidation.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <unistd.h>

static std::vector<std::string> logBuffer;

class ImGuiStreamBuf : public std::streambuf
{
public:
  std::string buffer;

protected:
  int overflow(int c) override
  {
    if (c == '\n')
    {
      logBuffer.push_back(buffer);
      buffer.clear();
    }
    else
    {
      buffer += static_cast<char>(c);
    }
    return c;
  }

  int sync() override
  {
    if (!buffer.empty())
    {
      logBuffer.push_back(buffer);
      buffer.clear();
    }
    return 0;
  }
};

// Global instance
static ImGuiStreamBuf imguiBuf;
static std::streambuf* oldCoutBuf = nullptr;
static std::streambuf* oldCerrBuf = nullptr;

void RedirectCppStreams()
{
  oldCoutBuf = std::cout.rdbuf(&imguiBuf);
  oldCerrBuf = std::cerr.rdbuf(&imguiBuf);
}

void RestoreCppStreams()
{
  std::cout.rdbuf(oldCoutBuf);
  std::cerr.rdbuf(oldCerrBuf);
}

// Buffer where we'll store logs
extern std::vector<std::string> logBuffer;
std::string pendingLine;

int pipefd[2];
std::thread stdoutThread;

void RedirectStdout()
{
  pipe(pipefd); // Create pipe: pipefd[0] = read, pipefd[1] = write

  // Redirect stdout to the pipe's write end
  dup2(pipefd[1], STDOUT_FILENO);
  // Optional: redirect stderr too
  // dup2(pipefd[1], STDERR_FILENO);

  // Set non-blocking read from pipe
  fcntl(pipefd[0], F_SETFL, O_NONBLOCK);

  // Background thread to read from pipe and push to ImGui buffer
  stdoutThread = std::thread(
    []()
    {
      char buffer[512];
      while (true)
      {
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (bytesRead > 0)
        {
          buffer[bytesRead] = '\0';
          std::string output(buffer);

          size_t pos = 0;
          while ((pos = output.find('\n')) != std::string::npos)
          {
            std::string line = output.substr(0, pos);
            logBuffer.push_back(pendingLine + line);
            pendingLine.clear();
            output.erase(0, pos + 1);
          }

          if (!output.empty())
          {
            pendingLine += output;
          }
        }
        else
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
      }
    });
}

// Then call RedirectOutputToImGui(); early in your app (before your main loop).
// 🪄 Use in Your Console Window

void ShowConsoleWindow()
{
  ImGui::Begin("Console");

  ImGui::BeginChild("scroll",
                    ImVec2(0, -ImGui::GetFrameHeightWithSpacing()),
                    true,
                    ImGuiWindowFlags_HorizontalScrollbar);
  for (const auto& line : logBuffer)
  {
    ImGui::TextUnformatted(line.c_str());
  }
  ImGui::EndChild();

  ImGui::End();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static void framebufferResizeCallback(GLFWwindow* window,
                                      int /*width*/,
                                      int /*height*/)
{
  auto app =
    reinterpret_cast<VulkanPipeLine*>(glfwGetWindowUserPointer(window));
  app->mFramebufferResized = true;
}

bool hasStencilComponent(VkFormat format)
{
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
         format == VK_FORMAT_D24_UNORM_S8_UINT;
}

static void check_vk_result(VkResult err)
{
  if (err == 0)
    return;
  fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
  if (err < 0)
    abort();
}

void VulkanPipeLine::createColorResources()
{
  VkFormat colorFormat = mVulkanContext.mSwapChainImageFormat;

  createImage(mVulkanContext.mSwapChainExtent.width,
              mVulkanContext.mSwapChainExtent.height,
              1,
              mVulkanContext.mMsaaSamples,
              colorFormat,
              VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
              mVulkanContext.mColorImage,
              mVulkanContext.mColorImageMemory);

  mVulkanContext.mColorImageView = createImageView(
    mVulkanContext.mColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

VkSampleCountFlagBits VulkanPipeLine::getMaxUsableSampleCount()
{
  VkPhysicalDeviceProperties physicalDeviceProperties;
  vkGetPhysicalDeviceProperties(mVulkanContext.mPhysicalDevice,
                                &physicalDeviceProperties);

  VkSampleCountFlags counts =
    physicalDeviceProperties.limits.framebufferColorSampleCounts &
    physicalDeviceProperties.limits.framebufferDepthSampleCounts;
  if (counts & VK_SAMPLE_COUNT_64_BIT)
  {
    return VK_SAMPLE_COUNT_64_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_32_BIT)
  {
    return VK_SAMPLE_COUNT_32_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_16_BIT)
  {
    return VK_SAMPLE_COUNT_16_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_8_BIT)
  {
    return VK_SAMPLE_COUNT_8_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_4_BIT)
  {
    return VK_SAMPLE_COUNT_4_BIT;
  }
  if (counts & VK_SAMPLE_COUNT_2_BIT)
  {
    return VK_SAMPLE_COUNT_2_BIT;
  }

  return VK_SAMPLE_COUNT_1_BIT;
}

void VulkanPipeLine::createImage(uint32_t width,
                                 uint32_t height,
                                 uint32_t aMipLevels,
                                 VkSampleCountFlagBits numSamples,
                                 VkFormat format,
                                 VkImageTiling tiling,
                                 VkImageUsageFlags usage,
                                 VkMemoryPropertyFlags properties,
                                 VkImage& image,
                                 VkDeviceMemory& imageMemory)
{
  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = aMipLevels;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.samples = numSamples;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateImage(mVulkanContext.mDevice, &imageInfo, nullptr, &image) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create image!");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(mVulkanContext.mDevice, image, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
    findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(
        mVulkanContext.mDevice, &allocInfo, nullptr, &imageMemory) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate image memory!");
  }

  vkBindImageMemory(mVulkanContext.mDevice, image, imageMemory, 0);
}

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

VkSurfaceFormatKHR chooseSwapSurfaceFormat(
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
VkFormat VulkanPipeLine::findSupportedFormat(
  const std::vector<VkFormat>& candidates,
  VkImageTiling tiling,
  VkFormatFeatureFlags features)
{
  for (VkFormat format : candidates)
  {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(
      mVulkanContext.mPhysicalDevice, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features)
    {
      return format;
    }
    else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
             (props.optimalTilingFeatures & features) == features)
    {
      return format;
    }
  }

  throw std::runtime_error("failed to find supported format!");
}

VkFormat VulkanPipeLine::findDepthFormat()
{
  return findSupportedFormat({ VK_FORMAT_D32_SFLOAT,
                               VK_FORMAT_D32_SFLOAT_S8_UINT,
                               VK_FORMAT_D24_UNORM_S8_UINT },
                             VK_IMAGE_TILING_OPTIMAL,
                             VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

uint32_t VulkanPipeLine::findMemoryType(uint32_t typeFilter,
                                        VkMemoryPropertyFlags properties)
{
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(mVulkanContext.mPhysicalDevice,
                                      &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
  {
    if ((typeFilter & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
    {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}

VkPresentModeKHR chooseSwapPresentMode(
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

void VulkanPipeLine::createBuffer(VkDeviceSize size,
                                  VkBufferUsageFlags usage,
                                  VkMemoryPropertyFlags properties,
                                  VkBuffer& buffer,
                                  VkDeviceMemory& bufferMemory)
{
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(mVulkanContext.mDevice, &bufferInfo, nullptr, &buffer) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(
    mVulkanContext.mDevice, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
    findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(
        mVulkanContext.mDevice, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate buffer memory!");
  }

  vkBindBufferMemory(mVulkanContext.mDevice, buffer, bufferMemory, 0);
}

void VulkanPipeLine::copyBuffer(VkBuffer srcBuffer,
                                VkBuffer dstBuffer,
                                VkDeviceSize size)
{
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = mVulkanContext.mCommandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(mVulkanContext.mDevice, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  VkBufferCopy copyRegion{};
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(mVulkanContext.mGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(mVulkanContext.mGraphicsQueue);

  vkFreeCommandBuffers(
    mVulkanContext.mDevice, mVulkanContext.mCommandPool, 1, &commandBuffer);
}

VkExtent2D VulkanPipeLine::chooseSwapExtent(
  const VkSurfaceCapabilitiesKHR& capabilities)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
  {
    return capabilities.currentExtent;
  }
  else
  {
    int width, height;
    glfwGetFramebufferSize(mWindow, &width, &height);

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

SwapChainSupportDetails VulkanPipeLine::querySwapChainSupport(
  VkPhysicalDevice aPhysicalDevice)
{
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    aPhysicalDevice, mVulkanContext.mSurface, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    aPhysicalDevice, mVulkanContext.mSurface, &formatCount, nullptr);

  if (formatCount != 0)
  {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(aPhysicalDevice,
                                         mVulkanContext.mSurface,
                                         &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    aPhysicalDevice, mVulkanContext.mSurface, &presentModeCount, nullptr);

  if (presentModeCount != 0)
  {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(aPhysicalDevice,
                                              mVulkanContext.mSurface,
                                              &presentModeCount,
                                              details.presentModes.data());
  }

  return details;
}

QueueFamilyIndices VulkanPipeLine::findQueueFamilies(VkPhysicalDevice aDevice)
{
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(aDevice, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
    aDevice, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies)
  {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(
      aDevice, i, mVulkanContext.mSurface, &presentSupport);

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

VkInstance VulkanPipeLine::GetInstance()
{
  return mVulkanContext.mInstance;
}

void VulkanPipeLine::createInstance()
{
  if (enableValidationLayers && !checkValidationLayerSupport())
  {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(21, 3, 7);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(21, 3, 7);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
#ifdef __APPLE__
  createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

  auto extensions = getRequiredExtensions();
#ifdef __APPLE__
  extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  extensions.emplace_back(
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (enableValidationLayers)
  {
    createInfo.enabledLayerCount =
      static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
  }
  else
  {
    createInfo.enabledLayerCount = 0;

    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &mVulkanContext.mInstance) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create instance!");
  }
}

void VulkanPipeLine::setupDebugMessenger()
{
  if (!enableValidationLayers)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);

  if (CreateDebugUtilsMessengerEXT(mVulkanContext.mInstance,
                                   &createInfo,
                                   nullptr,
                                   &mVulkanContext.mDebugMessenger) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to set up debug messenger!");
  }
}

void VulkanPipeLine::createSurface()
{
  VkResult result = glfwCreateWindowSurface(
    mVulkanContext.mInstance, mWindow, nullptr, &mVulkanContext.mSurface);
  if (result != VK_SUCCESS)
  {
    printf("Error : %i \n", result);
    throw std::runtime_error("failed to create window surface!");
  }
}

void VulkanPipeLine::createSwapChain()
{
  SwapChainSupportDetails swapChainSupport =
    querySwapChainSupport(mVulkanContext.mPhysicalDevice);

  VkSurfaceFormatKHR surfaceFormat =
    chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
    chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount)
  {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = mVulkanContext.mSurface;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices =
    findQueueFamilies(mVulkanContext.mPhysicalDevice);
  uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(),
                                    indices.presentFamily.value() };

  if (indices.graphicsFamily != indices.presentFamily)
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  }
  else
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(mVulkanContext.mDevice,
                           &createInfo,
                           nullptr,
                           &mVulkanContext.mSwapChain) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(
    mVulkanContext.mDevice, mVulkanContext.mSwapChain, &imageCount, nullptr);
  mVulkanContext.mSwapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(mVulkanContext.mDevice,
                          mVulkanContext.mSwapChain,
                          &imageCount,
                          mVulkanContext.mSwapChainImages.data());

  mVulkanContext.mSwapChainImageFormat = surfaceFormat.format;
  mVulkanContext.mSwapChainExtent = extent;
}

void VulkanPipeLine::createImageViews()
{
  mVulkanContext.mSwapChainImageViews.resize(
    mVulkanContext.mSwapChainImages.size());

  for (uint32_t i = 0; i < mVulkanContext.mSwapChainImages.size(); i++)
  {
    mVulkanContext.mSwapChainImageViews[i] =
      createImageView(mVulkanContext.mSwapChainImages[i],
                      mVulkanContext.mSwapChainImageFormat,
                      VK_IMAGE_ASPECT_COLOR_BIT,
                      1);
  }
}

void VulkanPipeLine::createRenderPass()
{
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = mVulkanContext.mSwapChainImageFormat;
  colorAttachment.samples = mVulkanContext.mMsaaSamples;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = findDepthFormat();
  depthAttachment.samples = mVulkanContext.mMsaaSamples;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout =
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription colorAttachmentResolve{};
  colorAttachmentResolve.format = mVulkanContext.mSwapChainImageFormat;
  colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference colorAttachmentResolveRef{};
  colorAttachmentResolveRef.attachment = 2;
  colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;
  subpass.pResolveAttachments = &colorAttachmentResolveRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  std::array<VkAttachmentDescription, 3> attachments = {
    colorAttachment, depthAttachment, colorAttachmentResolve
  };
  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if (vkCreateRenderPass(mVulkanContext.mDevice,
                         &renderPassInfo,
                         nullptr,
                         &mVulkanContext.mRenderPass) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create render pass!");
  }
}

void VulkanPipeLine::createDescriptorSetLayout()
{
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.pImmutableSamplers = nullptr;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 1;
  samplerLayoutBinding.descriptorCount = 1;
  samplerLayoutBinding.descriptorType =
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = nullptr;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
    uboLayoutBinding, samplerLayoutBinding
  };

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings = bindings.data();

  if (vkCreateDescriptorSetLayout(mVulkanContext.mDevice,
                                  &layoutInfo,
                                  nullptr,
                                  &mVulkanContext.mDescriptorSetLayout) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create descriptor set layout!");
  }
}

void VulkanPipeLine::createGraphicsPipeline()
{

  std::unique_ptr<VulkanShader> vertexShader = std::make_unique<VulkanShader>(
    "Vertex Shader",
    "shaders/vert.spv",
    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
    mVulkanContext.mDevice);

  std::unique_ptr<VulkanShader> fragmentShader = std::make_unique<VulkanShader>(
    "Fragment Shader",
    "shaders/frag.spv",
    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
    mVulkanContext.mDevice);

  VkPipelineShaderStageCreateInfo shaderStages[] = {
    vertexShader->ShaderStageInfoRef(), fragmentShader->ShaderStageInfoRef()
  };

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  auto bindingDescription = Vertex::getBindingDescription();
  auto attributeDescriptions = Vertex::getAttributeDescriptions();

  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.vertexAttributeDescriptionCount =
    static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = mVulkanContext.mMsaaSamples;
  multisampling.sampleShadingEnable =
    VK_TRUE; // enable sample shading in the pipeline
  multisampling.minSampleShading =
    .2f; // min fraction for sample shading; closer to one is smoother

  VkPipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.sType =
    VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.stencilTestEnable = VK_FALSE;

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType =
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT,
                                                VK_DYNAMIC_STATE_SCISSOR };
  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
  dynamicState.pDynamicStates = dynamicStates.data();

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &mVulkanContext.mDescriptorSetLayout;

  /*Push constants*/
  // setup push constants
  VkPushConstantRange push_constant;
  // this push constant range starts at the beginning
  push_constant.offset = 0;
  // this push constant range takes up the size of a MeshPushConstants struct
  push_constant.size = sizeof(MeshPushConstants);
  // this push constant range is accessible only in the vertex shader
  push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  pipelineLayoutInfo.pPushConstantRanges = &push_constant;
  pipelineLayoutInfo.pushConstantRangeCount = 1;

  if (vkCreatePipelineLayout(mVulkanContext.mDevice,
                             &pipelineLayoutInfo,
                             nullptr,
                             &mVulkanContext.mPipelineLayout) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create pipeline layout!");
  }

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = &depthStencil;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = mVulkanContext.mPipelineLayout;
  pipelineInfo.renderPass = mVulkanContext.mRenderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(mVulkanContext.mDevice,
                                VK_NULL_HANDLE,
                                1,
                                &pipelineInfo,
                                nullptr,
                                &mVulkanContext.mGraphicsPipeline) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create graphics pipeline!");
  }

  fragmentShader->DestroyShaderModule();
  vertexShader->DestroyShaderModule();
}

void VulkanPipeLine::createFramebuffers()
{
  mVulkanContext.mSwapChainFramebuffers.resize(
    mVulkanContext.mSwapChainImageViews.size());

  for (size_t i = 0; i < mVulkanContext.mSwapChainImageViews.size(); i++)
  {
    std::array<VkImageView, 3> attachments = {
      mVulkanContext.mColorImageView,
      mVulkanContext.mDepthImageView,
      mVulkanContext.mSwapChainImageViews[i]
    };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = mVulkanContext.mRenderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = mVulkanContext.mSwapChainExtent.width;
    framebufferInfo.height = mVulkanContext.mSwapChainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(mVulkanContext.mDevice,
                            &framebufferInfo,
                            nullptr,
                            &mVulkanContext.mSwapChainFramebuffers[i]) !=
        VK_SUCCESS)
    {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

void VulkanPipeLine::createCommandPool()
{
  QueueFamilyIndices queueFamilyIndices =
    findQueueFamilies(mVulkanContext.mPhysicalDevice);

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  if (vkCreateCommandPool(mVulkanContext.mDevice,
                          &poolInfo,
                          nullptr,
                          &mVulkanContext.mCommandPool) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create command pool!");
  }
}

void VulkanPipeLine::createDepthResources()
{
  VkFormat depthFormat = findDepthFormat();
  createImage(mVulkanContext.mSwapChainExtent.width,
              mVulkanContext.mSwapChainExtent.height,
              1,
              mVulkanContext.mMsaaSamples,
              depthFormat,
              VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
              mVulkanContext.mDepthImage,
              mVulkanContext.mDepthImageMemory);
  mVulkanContext.mDepthImageView = createImageView(
    mVulkanContext.mDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}

VkImageView VulkanPipeLine::createImageView(VkImage image,
                                            VkFormat format,
                                            VkImageAspectFlags aspectFlags,
                                            uint32_t aMipLevel)
{
  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = aMipLevel;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  VkImageView imageView;
  if (vkCreateImageView(
        mVulkanContext.mDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create texture image view!");
  }

  return imageView;
}

void VulkanPipeLine::setupImgui()
{

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
    ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
    ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  ImGui::StyleColorsDark();

  SwapChainSupportDetails swapChainSupport =
    querySwapChainSupport(mVulkanContext.mPhysicalDevice);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount)
  {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  QueueFamilyIndices indices =
    findQueueFamilies(mVulkanContext.mPhysicalDevice);

  VkDescriptorPoolSize pool_sizes[] = {
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
  };
  VkDescriptorPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  pool_info.maxSets = 1;
  pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
  pool_info.pPoolSizes = pool_sizes;
  vkCreateDescriptorPool(mVulkanContext.mDevice,
                         &pool_info,
                         mVulkanContext.g_Allocator,
                         &mVulkanContext.mImguiDescriptorPool);

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForVulkan(mWindow, true);
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = mVulkanContext.mInstance;
  init_info.PhysicalDevice = mVulkanContext.mPhysicalDevice;
  init_info.Device = mVulkanContext.mDevice;
  init_info.QueueFamily = indices.graphicsFamily.value();
  init_info.Queue = mVulkanContext.mGraphicsQueue;
  // init_info.PipelineCache = g_PipelineCache;
  init_info.DescriptorPool = mVulkanContext.mImguiDescriptorPool;
  init_info.RenderPass = mVulkanContext.mRenderPass;
  init_info.Subpass = 0;
  init_info.MinImageCount = swapChainSupport.capabilities.minImageCount;
  init_info.ImageCount = imageCount;
  init_info.MSAASamples = getMaxUsableSampleCount();
  init_info.Allocator = mVulkanContext.g_Allocator;
  init_info.CheckVkResultFn = check_vk_result;

  ImGui_ImplVulkan_Init(&init_info);
}

void VulkanPipeLine::createCommandBuffers()
{
  mVulkanContext.mCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = mVulkanContext.mCommandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount =
    (uint32_t)mVulkanContext.mCommandBuffers.size();

  if (vkAllocateCommandBuffers(mVulkanContext.mDevice,
                               &allocInfo,
                               mVulkanContext.mCommandBuffers.data()) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void VulkanPipeLine::createSyncObjects()
{
  mVulkanContext.mImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  mVulkanContext.mRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  mVulkanContext.mInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    if (vkCreateSemaphore(mVulkanContext.mDevice,
                          &semaphoreInfo,
                          nullptr,
                          &mVulkanContext.mImageAvailableSemaphores[i]) !=
          VK_SUCCESS ||
        vkCreateSemaphore(mVulkanContext.mDevice,
                          &semaphoreInfo,
                          nullptr,
                          &mVulkanContext.mRenderFinishedSemaphores[i]) !=
          VK_SUCCESS ||
        vkCreateFence(mVulkanContext.mDevice,
                      &fenceInfo,
                      nullptr,
                      &mVulkanContext.mInFlightFences[i]) != VK_SUCCESS)
    {
      throw std::runtime_error(
        "failed to create synchronization objects for a frame!");
    }
  }
}

void VulkanPipeLine::SetAppPtr(VulkanApp* aApp)
{
  mApp = aApp;
}

VulkanPipeLine::VulkanPipeLine(unsigned int width, unsigned int height)
  : mWidth(width)
  , mHeight(height)
  , demoSelectionValue(1)

{
  RedirectCppStreams();
  RedirectStdout();

  demoMap = DemoFactory::getDemoMap();
  for (auto& demo : demoMap)
  {
    demoNames.push_back(demo.second.second);
  }
  initWindow(width, height);
  initVulkan();
}

GLFWwindow* VulkanPipeLine::GetWindow()
{
  return mWindow;
}

void VulkanPipeLine::initWindow(int width, int height)
{

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  mWindow = glfwCreateWindow(width, height, "Vulkan window", nullptr, nullptr);
  glfwSetWindowUserPointer(mWindow, this);
  glfwSetFramebufferSizeCallback(mWindow, framebufferResizeCallback);
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
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

bool VulkanPipeLine::isDeviceSuitable(VkPhysicalDevice aDevice)
{
  QueueFamilyIndices indices = findQueueFamilies(aDevice);

  bool extensionsSupported = checkDeviceExtensionSupport(aDevice);

  bool swapChainAdequate = false;
  if (extensionsSupported)
  {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(aDevice);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  VkPhysicalDeviceFeatures supportedFeatures;
  vkGetPhysicalDeviceFeatures(aDevice, &supportedFeatures);

  return indices.isComplete() && extensionsSupported && swapChainAdequate &&
         supportedFeatures.samplerAnisotropy;
}

void VulkanPipeLine::pickPhysicalDevice()
{
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(mVulkanContext.mInstance, &deviceCount, nullptr);

  if (deviceCount == 0)
  {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(
    mVulkanContext.mInstance, &deviceCount, devices.data());

  for (const auto& device : devices)
  {
    if (isDeviceSuitable(device))
    {
      mVulkanContext.mPhysicalDevice = device;
      mVulkanContext.mMsaaSamples = getMaxUsableSampleCount();

      printf("Max Usable Msaa sample count:  %u .\n",
             mVulkanContext.mMsaaSamples);
      break;
    }
  }

  if (mVulkanContext.mPhysicalDevice == VK_NULL_HANDLE)
  {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

void VulkanPipeLine::createLogicalDevice()
{
  QueueFamilyIndices indices =
    findQueueFamilies(mVulkanContext.mPhysicalDevice);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(),
                                             indices.presentFamily.value() };

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies)
  {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.sampleRateShading = VK_TRUE;

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.queueCreateInfoCount =
    static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount =
    static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  if (enableValidationLayers)
  {
    createInfo.enabledLayerCount =
      static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  }
  else
  {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(mVulkanContext.mPhysicalDevice,
                     &createInfo,
                     nullptr,
                     &mVulkanContext.mDevice) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(mVulkanContext.mDevice,
                   indices.graphicsFamily.value(),
                   0,
                   &mVulkanContext.mGraphicsQueue);
  vkGetDeviceQueue(mVulkanContext.mDevice,
                   indices.presentFamily.value(),
                   0,
                   &mVulkanContext.mPresentQueue);
}

void VulkanPipeLine::initVulkan()
{

  if (!glfwVulkanSupported())
  {
    printf("GLFW failed to find the Vulkan loader.\nExiting ...\n");
  }
  createInstance();
  setupDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
  createRenderPass();
  createDescriptorSetLayout();
  createGraphicsPipeline();
  createCommandPool();
  createTextureSampler();

  createColorResources();
  createDepthResources();
  createFramebuffers();
}

void VulkanPipeLine::recreateSwapChain()
{
  int width = 0, height = 0;
  glfwGetFramebufferSize(mWindow, &width, &height);
  while (width == 0 || height == 0)
  {
    glfwGetFramebufferSize(mWindow, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(mVulkanContext.mDevice);

  cleanupSwapChain();

  createSwapChain();
  createImageViews();
  createColorResources();
  createDepthResources();
  createFramebuffers();
}

void VulkanPipeLine::updateUniformBuffer(uint32_t currentImage)
{
  UniformBufferObject ubo{};
  ubo.proj[1][1] *= -1; // OpenGl to Vulkan transition

  memcpy(mVulkanContext.mUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void VulkanPipeLine::DrawFrame()
{

  vkWaitForFences(mVulkanContext.mDevice,
                  1,
                  &mVulkanContext.mInFlightFences[mCurrentFrame],
                  VK_TRUE,
                  UINT64_MAX);

  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(
    mVulkanContext.mDevice,
    mVulkanContext.mSwapChain,
    UINT64_MAX,
    mVulkanContext.mImageAvailableSemaphores[mCurrentFrame],
    VK_NULL_HANDLE,
    &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    recreateSwapChain();
    return;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  vkResetFences(
    mVulkanContext.mDevice, 1, &mVulkanContext.mInFlightFences[mCurrentFrame]);

  vkResetCommandBuffer(mVulkanContext.mCommandBuffers[mCurrentFrame],
                       /*VkCommandBufferResetFlagBits*/ 0);
  recordCommandBuffer(mVulkanContext.mCommandBuffers[mCurrentFrame],
                      imageIndex);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {
    mVulkanContext.mImageAvailableSemaphores[mCurrentFrame]
  };
  VkPipelineStageFlags waitStages[] = {
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
  };
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &mVulkanContext.mCommandBuffers[mCurrentFrame];

  VkSemaphore signalSemaphores[] = {
    mVulkanContext.mRenderFinishedSemaphores[mCurrentFrame]
  };
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(mVulkanContext.mGraphicsQueue,
                    1,
                    &submitInfo,
                    mVulkanContext.mInFlightFences[mCurrentFrame]) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = { mVulkanContext.mSwapChain };
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;

  presentInfo.pImageIndices = &imageIndex;

  result = vkQueuePresentKHR(mVulkanContext.mPresentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      mFramebufferResized)
  {
    mFramebufferResized = false;
    recreateSwapChain();
  }
  else if (result != VK_SUCCESS)
  {
    throw std::runtime_error("failed to present swap chain image!");
  }

  mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanPipeLine::createDescriptorPool(unsigned aNumberOfModels)
{
  std::vector<VkDescriptorPoolSize> poolSizes{};
  poolSizes.resize(2 * aNumberOfModels);
  for (unsigned i = 0; i < aNumberOfModels; i++)
  {
    poolSizes[i * 2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[i * 2].descriptorCount =
      static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[(i * 2) + 1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[(i * 2) + 1].descriptorCount =
      static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
  }

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets =
    static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * aNumberOfModels);

  if (vkCreateDescriptorPool(mVulkanContext.mDevice,
                             &poolInfo,
                             nullptr,
                             &mVulkanContext.mDescriptorPool) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

void VulkanPipeLine::createTextureSampler()
{
  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(mVulkanContext.mPhysicalDevice, &properties);

  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_TRUE;
  samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.minLod = 0.0f; // Optional
  samplerInfo.maxLod = static_cast<float>(mVulkanContext.mMipLevels);
  samplerInfo.mipLodBias = 0.0f; // Optional

  if (vkCreateSampler(mVulkanContext.mDevice,
                      &samplerInfo,
                      nullptr,
                      &mVulkanContext.mTextureSampler) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create texture sampler!");
  }
}

void VulkanPipeLine::createUniformBuffers()
{
  VkDeviceSize bufferSize = sizeof(UniformBufferObject);

  mVulkanContext.mUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
  mVulkanContext.mUniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
  mVulkanContext.mUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 mVulkanContext.mUniformBuffers[i],
                 mVulkanContext.mUniformBuffersMemory[i]);

    vkMapMemory(mVulkanContext.mDevice,
                mVulkanContext.mUniformBuffersMemory[i],
                0,
                bufferSize,
                0,
                &mVulkanContext.mUniformBuffersMapped[i]);
  }
}

void ClearConsole()
{
  logBuffer.clear();
}

void VulkanPipeLine::drawImgui(VkCommandBuffer commandBuffer)
{

  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiIO& io = ImGui::GetIO();

  // 2. Show a simple window that we create ourselves. We use a Begin/End pair
  // to create a named window.
  {

    ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and
                                   // append into it.
    ImGui::Text("This is some useful text."); // Display some text (you can use
                                              // a format strings too)

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate);
    ImGui::End();

    // Demo Selection window
    ImGui::Begin("Demo Window");
    ImGui::Text("Choose a demo:");
    if (!demoNames.empty())
    {
      const char* preview = demoNames[demoSelectionValue - 1].empty()
                              ? "Unnamed"
                              : demoNames[demoSelectionValue - 1].c_str();
      // Combo box
      ImGui::PushItemWidth(200); // optional: fix width
      if (ImGui::BeginCombo("Select Option##demo_combo", preview))
      {
        for (int i = 0; i < demoNames.size(); ++i)
        {
          bool is_selected = (demoSelectionValue == i);
          const char* item_label =
            demoNames[i].empty() ? "Unnamed" : demoNames[i].c_str();

          if (ImGui::Selectable(item_label, is_selected))
          {
            demoSelectionValue = i + 1;
          }
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();
      ImGui::SameLine(); // <-- place button next to combo
      if (ImGui::Button("Run"))
      {
        auto demo =
          DemoFactory::createDemo(static_cast<DemoType>(demoSelectionValue));
        // Call your function here
        demo->ShowDemo();
      }

      ImGui::Text("You selected: %s", preview);
    }
    else
    {
      ImGui::Text("No demos available.");
    }
    ImGui::End();

    ImGui::Begin("Console");
    ImGui::BeginChild("scroll",
                      ImVec2(0, -ImGui::GetFrameHeightWithSpacing()),
                      true,
                      ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& line : logBuffer)
    {
      ImGui::TextUnformatted(line.c_str());
    }
    ImGui::EndChild();
    // Clear button
    if (ImGui::Button("Clear"))
    {
      ClearConsole();
    }

    ImGui::End();
  }

  ImGui::Render();
  ImDrawData* draw_data = ImGui::GetDrawData();

  ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);
}

void VulkanPipeLine::recordCommandBuffer(VkCommandBuffer commandBuffer,
                                         uint32_t imageIndex)
{
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = mVulkanContext.mRenderPass;
  renderPassInfo.framebuffer =
    mVulkanContext.mSwapChainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = { 0, 0 };
  renderPassInfo.renderArea.extent = mVulkanContext.mSwapChainExtent;

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = { { 0.0f, 0.0f, 0.9f, 1.0f } };
  clearValues[1].depthStencil = { 1.0f, 0 };

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(
    commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    mVulkanContext.mGraphicsPipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)mVulkanContext.mSwapChainExtent.width;
  viewport.height = (float)mVulkanContext.mSwapChainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = { 0, 0 };
  scissor.extent = mVulkanContext.mSwapChainExtent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  updateUniformBuffer(mCurrentFrame);

  drawImgui(commandBuffer);

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void VulkanPipeLine::cleanupSwapChain()
{
  vkDestroyImageView(
    mVulkanContext.mDevice, mVulkanContext.mColorImageView, nullptr);
  vkDestroyImage(mVulkanContext.mDevice, mVulkanContext.mColorImage, nullptr);
  vkFreeMemory(
    mVulkanContext.mDevice, mVulkanContext.mColorImageMemory, nullptr);

  vkDestroyImageView(
    mVulkanContext.mDevice, mVulkanContext.mDepthImageView, nullptr);
  vkDestroyImage(mVulkanContext.mDevice, mVulkanContext.mDepthImage, nullptr);
  vkFreeMemory(
    mVulkanContext.mDevice, mVulkanContext.mDepthImageMemory, nullptr);

  for (auto framebuffer : mVulkanContext.mSwapChainFramebuffers)
  {
    vkDestroyFramebuffer(mVulkanContext.mDevice, framebuffer, nullptr);
  }

  for (auto imageView : mVulkanContext.mSwapChainImageViews)
  {
    vkDestroyImageView(mVulkanContext.mDevice, imageView, nullptr);
  }

  vkDestroySwapchainKHR(
    mVulkanContext.mDevice, mVulkanContext.mSwapChain, nullptr);
}

VulkanContext& VulkanPipeLine::GetVulkanContext()
{
  return mVulkanContext;
}

void VulkanPipeLine::CleanUp()
{
  cleanupSwapChain();

  vkDestroySampler(
    mVulkanContext.mDevice, mVulkanContext.mTextureSampler, nullptr);

  vkDestroyPipeline(
    mVulkanContext.mDevice, mVulkanContext.mGraphicsPipeline, nullptr);
  vkDestroyPipelineLayout(
    mVulkanContext.mDevice, mVulkanContext.mPipelineLayout, nullptr);
  vkDestroyRenderPass(
    mVulkanContext.mDevice, mVulkanContext.mRenderPass, nullptr);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroyBuffer(
      mVulkanContext.mDevice, mVulkanContext.mUniformBuffers[i], nullptr);
    vkFreeMemory(
      mVulkanContext.mDevice, mVulkanContext.mUniformBuffersMemory[i], nullptr);
  }

  vkDestroyDescriptorPool(
    mVulkanContext.mDevice, mVulkanContext.mImguiDescriptorPool, nullptr);
  vkDestroyDescriptorPool(
    mVulkanContext.mDevice, mVulkanContext.mDescriptorPool, nullptr);

  vkDestroyDescriptorSetLayout(
    mVulkanContext.mDevice, mVulkanContext.mDescriptorSetLayout, nullptr);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroySemaphore(mVulkanContext.mDevice,
                       mVulkanContext.mRenderFinishedSemaphores[i],
                       nullptr);
    vkDestroySemaphore(mVulkanContext.mDevice,
                       mVulkanContext.mImageAvailableSemaphores[i],
                       nullptr);
    vkDestroyFence(
      mVulkanContext.mDevice, mVulkanContext.mInFlightFences[i], nullptr);
  }
  vkDestroyCommandPool(
    mVulkanContext.mDevice, mVulkanContext.mCommandPool, nullptr);

  vkDestroyDevice(mVulkanContext.mDevice, nullptr);

  if (enableValidationLayers)
  {
    DestroyDebugUtilsMessengerEXT(
      mVulkanContext.mInstance, mVulkanContext.mDebugMessenger, nullptr);
  }

  vkDestroySurfaceKHR(
    mVulkanContext.mInstance, mVulkanContext.mSurface, nullptr);
  vkDestroyInstance(mVulkanContext.mInstance, nullptr);

  glfwDestroyWindow(mWindow);
}
