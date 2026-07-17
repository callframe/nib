#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <cstdlib>
#include <utility>

#include "nib/event_loop.h"
#include "nib/renderer.h"

class Nib : public nib::Application
{
 public:
  Nib(nib::Renderer* renderer)
      : surface(VK_NULL_HANDLE), device(std::nullopt), renderer(std::move(renderer))
  {}

  void handle_init(nib::Event_Loop& event_loop) override
  {
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(event_loop.get_window(), this->renderer->get_instance(), nullptr,
                                  &surface))
    {
      std::fprintf(stderr, "Failed to create Vulkan surface: %s\n", SDL_GetError());
      event_loop.exit();
      return;
    }

    auto device_res = nib::Device::new_(this->renderer->get_instance(), surface);
    if (!device_res)
    {
      std::fprintf(stderr, "Failed to create Vulkan device\n");
      event_loop.exit();
      vkDestroySurfaceKHR(renderer->get_instance(), surface, nullptr);
      return;
    }

    this->surface = surface;
    this->device = device_res.unwrap();
    this->renderer->set_device(&this->device.value());
  }

  void handle_event(nib::Event_Loop& event_loop, SDL_Event event) override
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

  ~Nib()
  {
    vkDestroySurfaceKHR(renderer->get_instance(), surface, nullptr);
    this->surface = VK_NULL_HANDLE;
  }

 private:
  VkSurfaceKHR surface;
  std::optional<nib::Device> device;
  nib::Renderer* renderer;
};

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    std::fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  auto renderer_res = nib::Renderer::new_();
  if (!renderer_res)
  {
    SDL_Quit();
    return EXIT_FAILURE;
  }

  nib::Renderer renderer = renderer_res.unwrap();
  Nib nib(&renderer);

  auto client_res = nib::Event_Loop::new_(&nib);
  if (!client_res)
  {
    SDL_Quit();
    return EXIT_FAILURE;
  }

  auto client = client_res.unwrap();
  client.run();

  SDL_Quit();
  return EXIT_SUCCESS;
}
