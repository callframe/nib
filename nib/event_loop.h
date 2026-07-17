#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

#include "nib/result.h"

namespace nib
{

class Event_Loop;

struct Application
{
  virtual void handle_init(Event_Loop& client) = 0;
  virtual void handle_event(Event_Loop& client, SDL_Event event) = 0;
};

enum class Event_Loop_Error
{
  Window_Creation_Failed,
};

class Event_Loop
{
 public:
  static Result<Event_Loop, Event_Loop_Error> new_(Application* app);
  ~Event_Loop() { SDL_DestroyWindow(this->window); }

  Event_Loop(const Event_Loop& other) = delete;
  Event_Loop& operator=(const Event_Loop& other) = delete;
  Event_Loop(Event_Loop&& other);

  void run();
  void exit() { running = false; }

  SDL_Window* get_window() const { return this->window; }

 private:
  Event_Loop(Application* app, SDL_Window* window) : running(true), app(app), window(window) {}

  bool running;
  Application* app;
  SDL_Window* window;
};

}  // namespace nib