#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "nib/app.h"
#include "nib/common.h"
#include "nib/event_loop.h"

constexpr std::uint32_t APP_VERSION = VK_MAKE_VERSION(1, 0, 0);

constexpr static VkApplicationInfo get_app_info()
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

static VkInstanceCreateInfo get_instance_create_info(VkApplicationInfo const& app_info)
{
  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  std::uint32_t count = 0;
  char const* const* extensions = SDL_Vulkan_GetInstanceExtensions(&count);
  create_info.enabledExtensionCount = count;
  create_info.ppEnabledExtensionNames = extensions;

  return create_info;
}

static VkInstance create_instance()
{
  VkApplicationInfo app_info = get_app_info();
  VkInstanceCreateInfo create_info = get_instance_create_info(app_info);

  VkInstance instance;
  VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
  if (result != VK_SUCCESS)
  {
    std::fprintf(stderr, "Failed to create Vulkan instance: %d\n", result);
    return VK_NULL_HANDLE;
  }

  return instance;
}

int main()
{
  int ec = EXIT_FAILURE;

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    std::fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  VkInstance instance = create_instance();
  if (instance == VK_NULL_HANDLE)
  {
    SDL_Quit();
    ec = EXIT_FAILURE;
    return ec;
  }

  {
    Nib nib(instance);

    auto client_res = nib::Event_Loop::new_(&nib);
    if (!client_res)
    {
      goto out;
    }

    auto client = client_res.unwrap();
    client.run();
  }

  ec = EXIT_SUCCESS;
out:
  vkDestroyInstance(instance, nullptr);
  SDL_Quit();
  return ec;
}
