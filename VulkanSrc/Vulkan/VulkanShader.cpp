#include "VulkanShader.hpp"

#include <vector>
#include <fstream>

static std::vector<char> readFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open())
  {
    std::string error = "failed to open file : " + filename;
    throw std::runtime_error(error);
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

VkShaderModule createShaderModule(const std::vector<char>& code,
                                  VkDevice aDevice)
{
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(aDevice, &createInfo, nullptr, &shaderModule) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create shader module!");
  }
  else
  {
    printf("Shader module created sucessfully.\n");
  }

  return shaderModule;
}

VulkanShader::VulkanShader(const std::string& aName,
                           const std::string& aPath,
                           VkStructureType aShaderType,
                           VkShaderStageFlagBits aShaderStage,
                           VkDevice aDevice)
  : mName(aName)
  , mShaderType(aShaderType)
  , mShaderStage(aShaderStage)
  , mDevice(aDevice)
{

  auto shaderCode = readFile(aPath);

  mShaderModule = createShaderModule(shaderCode, mDevice);

  mShaderStageInfo = VkPipelineShaderStageCreateInfo{};
  mShaderStageInfo.sType = aShaderType;
  mShaderStageInfo.stage = aShaderStage;
  mShaderStageInfo.module = mShaderModule;
  mShaderStageInfo.pName = "main";
}
