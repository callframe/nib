#include "nib/event_loop.h"

#include <cstdio>
#include <glm/glm.hpp>

#include "nib/common.h"

constexpr static glm::ivec2 WINDOW_SIZE = glm::ivec2(800, 600);

nib::Result<nib::Event_Loop, nib::Event_Loop_Error> nib::Event_Loop::new_(Application* app)
{
  SDL_Window* window = SDL_CreateWindow(APP_NAME, WINDOW_SIZE.x, WINDOW_SIZE.y, 0);
  if (!window)
  {
    std::fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return Event_Loop_Error::Window_Creation_Failed;
  }

  return Event_Loop(app, window);
}

nib::Event_Loop::Event_Loop(Event_Loop&& other)
    : running(other.running), app(other.app), window(other.window)
{
  other.running = false;
  other.app = nullptr;
  other.window = nullptr;
}

void nib::Event_Loop::run()
{
  this->app->handle_init(*this);

  while (this->running)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      this->app->handle_event(*this, event);
    }
  }
}
