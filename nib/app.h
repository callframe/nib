#pragma once

#include <vulkan/vulkan.h>

#include <optional>

#include "nib/device.h"
#include "nib/event_loop.h"

class Nib : public nib::Application
{
 public:
  Nib(VkInstance instance) : surface(VK_NULL_HANDLE), device(std::nullopt), instance(instance) {}

  void handle_init(nib::Event_Loop& event_loop) override;
  void handle_event(nib::Event_Loop& event_loop, SDL_Event event) override;

  ~Nib() { vkDestroySurfaceKHR(this->instance, surface, nullptr); }

 private:
  VkSurfaceKHR surface;
  std::optional<nib::Device> device;
  VkInstance instance;
};
