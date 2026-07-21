#include "nib/vulkan.h"

#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan_core.h>

#include <cstdint>

namespace nib
{

void init_render_target(entt::registry& registry, entt::entity window, SDL_Window* sdl_window)
{
  auto& context = registry.ctx().get<Vulkan_Context>();

  VkSurfaceKHR surface;
  if (!SDL_Vulkan_CreateSurface(sdl_window, context.instance, nullptr, &surface))
  {
    throw std::runtime_error("Failed to create Vulkan surface");
  }

  Render_Target render_target{surface};
  registry.emplace<Render_Target>(window, std::move(render_target));
}

void drop_render_target(entt::registry& registry, entt::entity window)
{
  auto& render_target = registry.get<Render_Target>(window);
  auto& context = registry.ctx().get<Vulkan_Context>();
  vkDestroySurfaceKHR(context.instance, render_target.surface, nullptr);
  registry.remove<Render_Target>(window);
}

static void fill_vk_instance_create_info(VkInstanceCreateInfo& create_info)
{
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pNext = nullptr;

  std::uint32_t extension_count = 0;
  char const* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extension_count);
  create_info.enabledExtensionCount = extension_count;
  create_info.ppEnabledExtensionNames = extensions;
}

void init_vulkan_context(entt::registry& registry)
{
  VkInstanceCreateInfo create_info{};
  fill_vk_instance_create_info(create_info);

  VkInstance instance;
  if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create Vulkan instance");
  }

  Vulkan_Context context{instance};
  registry.ctx().emplace<Vulkan_Context>(std::move(context));
}

void drop_vulkan_context(entt::registry& registry)
{
  auto& context = registry.ctx().get<Vulkan_Context>();
  vkDestroyInstance(context.instance, nullptr);
  registry.ctx().erase<Vulkan_Context>();
}

}  // namespace nib