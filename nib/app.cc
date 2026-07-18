#include "nib/app.h"

#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <cstdio>

#include "nib/device.h"
#include "nib/event_loop.h"

void Nib::handle_init(nib::Event_Loop& event_loop)
{
  VkSurfaceKHR surface;
  if (!SDL_Vulkan_CreateSurface(event_loop.get_window(), this->instance, nullptr, &surface))
  {
    std::fprintf(stderr, "Failed to create Vulkan surface: %s\n", SDL_GetError());
    event_loop.exit();
    return;
  }

  auto device_res = nib::Device::new_(this->instance, surface);
  if (!device_res)
  {
    std::fprintf(stderr, "Failed to create Vulkan device\n");
    event_loop.exit();
    vkDestroySurfaceKHR(this->instance, surface, nullptr);
    return;
  }

  this->surface = surface;
  this->device = device_res.unwrap();
}

void Nib::handle_event(nib::Event_Loop& event_loop, SDL_Event event)
{
  switch (event.type)
  {
    case SDL_EVENT_QUIT:
      event_loop.exit();
      break;
    default:
      break;
  }
}
