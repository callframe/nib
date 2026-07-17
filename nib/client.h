#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include "nib/result.h"

namespace nib
{

class Client;

struct IApp
{
  virtual void handle_init(Client& client) = 0;
  virtual void handle_event(Client& client, SDL_Event event) = 0;
};

enum class Client_Error
{
  Sdl_Init_Failed,
  Failed_To_Create_Window,
};

class Client
{
 public:
  static Result<Client, Client_Error> new_(IApp* app);
  ~Client();

  Client(const Client& other) = delete;
  Client& operator=(const Client& other) = delete;
  Client(Client&& other);

  void run();
  void exit() { running = false; }

 private:
  Client(IApp* app, SDL_Window* window) : running(true), app(app), window(window) {}

  bool running;
  IApp* app;
  SDL_Window* window;
};

}  // namespace nib