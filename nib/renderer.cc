#include "nib/renderer.h"

#include <cstdio>

#include "nib/common.h"

constexpr uint32_t APP_VERSION = VK_MAKE_VERSION(1, 0, 0);

constexpr static VkApplicationInfo renderer_app_info()
{
  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = nib::APP_NAME;
  app_info.applicationVersion = APP_VERSION;
  app_info.pEngineName = nib::APP_NAME;
  app_info.engineVersion = APP_VERSION;
  app_info.apiVersion = VK_API_VERSION_1_3;
  return app_info;
}

constexpr static VkInstanceCreateInfo renderer_instance_create_info(
    VkApplicationInfo const& app_info)
{
  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  return create_info;
}

nib::Result<nib::Renderer, nib::Renderer_Error> nib::Renderer::new_()
{
  VkApplicationInfo app_info = renderer_app_info();
  VkInstanceCreateInfo create_info = renderer_instance_create_info(app_info);

  VkInstance instance;
  VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
  if (result != VK_SUCCESS)
  {
    std::fprintf(stderr, "Failed to create Vulkan instance: %d\n", result);
    return Renderer_Error::Failed_To_Create_VkInstance;
  }

  return Renderer(instance);
}

nib::Renderer::~Renderer() { vkDestroyInstance(instance, nullptr); }