#pragma once

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <entt/entt.hpp>

namespace nib
{

struct Render_Target
{
  VkSurfaceKHR surface;
};

void init_render_target(entt::registry& registry, entt::entity window, SDL_Window* sdl_window);
void drop_render_target(entt::registry& registry, entt::entity window);

struct Vulkan_Context
{
  VkInstance instance;
};

void init_vulkan_context(entt::registry& registry);
void drop_vulkan_context(entt::registry& registry);

}  // namespace nib