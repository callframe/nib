#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <cstdio>
#include <cstdlib>

#include "nib/common.h"
#include "nib/defer.h"
#include "nib/result.h"

using namespace nib;

static Result<Unit, Unit> real_main()
{
  SDL_Window* window =
      SDL_CreateWindow(APP_NAME, APP_WINDOW_SIZE.x, APP_WINDOW_SIZE.y, SDL_WINDOW_VULKAN);
  if (!window)
  {
    std::fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return Err<Unit>({});
  }

  NIB_DEFER(SDL_DestroyWindow(window));

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
  }
}

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    std::fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  int ec = real_main().is_ok ? EXIT_SUCCESS : EXIT_FAILURE;
  SDL_Quit();
  return ec;
}
