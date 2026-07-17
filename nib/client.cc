#include "nib/client.h"

#include <cstdio>
#include <glm/glm.hpp>

#include "nib/common.h"

constexpr static glm::ivec2 WINDOW_SIZE = glm::ivec2(800, 600);

nib::Result<nib::Client, nib::Client_Error> nib::Client::new_(IApp* app)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    std::fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return Client_Error::Sdl_Init_Failed;
  }

  SDL_Window* window = SDL_CreateWindow(APP_NAME, WINDOW_SIZE.x, WINDOW_SIZE.y, 0);
  if (!window)
  {
    std::fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return Client_Error::Failed_To_Create_Window;
  }

  return Client(app, window);
}

nib::Client::Client(Client&& other) : running(other.running), app(other.app), window(other.window)
{
  other.running = false;
  other.app = nullptr;
  other.window = nullptr;
}

void nib::Client::run()
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

nib::Client::~Client()
{
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}