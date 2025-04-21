#include "VulkanApp.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "VulkanPipeline.hpp"

VulkanApp::VulkanApp(VulkanPipeLine& aGraphicPipeline)
  : mGraphicPipeline(aGraphicPipeline)
{
  mGraphicPipeline.SetAppPtr(this);
}

void VulkanApp::Run()
{
  mGraphicPipeline.createDescriptorPool(1000);
  mGraphicPipeline.createUniformBuffers();

  mGraphicPipeline.createCommandBuffers();
  mGraphicPipeline.createSyncObjects();
  mGraphicPipeline.setupImgui();

  while (!glfwWindowShouldClose(mGraphicPipeline.GetWindow()))
  {
    glfwPollEvents();

    // Draw stuff
    {
      mGraphicPipeline.DrawFrame();
    }
  }
  vkDeviceWaitIdle(mGraphicPipeline.GetDevice());
  CleanUp();
}

void VulkanApp::CleanUp()
{
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  mGraphicPipeline.CleanUp();
  glfwTerminate();
}

VulkanApp::~VulkanApp()
{
}
