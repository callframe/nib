#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <cstdio>
#include <cstdlib>
#include <entt/entt.hpp>

#include "nib/common.h"
#include "nib/defer.h"
#include "nib/result.h"
#include "nib/vulkan.h"

using namespace nib;

static Result<Unit, Unit> real_main(entt::registry& registry)
{
  SDL_Window* window =
      SDL_CreateWindow(APP_NAME, APP_WINDOW_SIZE.x, APP_WINDOW_SIZE.y, SDL_WINDOW_VULKAN);
  if (!window)
  {
    std::fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return Err<Unit>({});
  }

  auto window_entity = registry.create();
  init_render_target(registry, window_entity, window);

  NIB_DEFER(drop_render_target(registry, window_entity));
  NIB_DEFER(SDL_DestroyWindow(window));

  entt::organizer organizer;

  auto graph = organizer.graph();
  for (auto&& node : graph)
  {
    node.prepare(registry);
  }

  while (true)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_EVENT_QUIT:
          return Ok<Unit>({});
        default:
          break;
      }
    }

    for (auto&& node : graph)
    {
      node.callback()(node.data(), registry);
    }
  }
}

int main()
{
  entt::registry registry;

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    std::fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  init_vulkan_context(registry);
  int ec = real_main(registry).is_ok ? EXIT_SUCCESS : EXIT_FAILURE;

  drop_vulkan_context(registry);
  SDL_Quit();
  return ec;
}
