#include <cstdlib>

#include "nib/client.h"
#include "nib/renderer.h"

class Nib : public nib::IApp
{
 public:
  Nib(nib::Renderer renderer) : renderer(std::move(renderer)) {}

  void handle_init(nib::Client& client) override { (void)client; }

  void handle_event(nib::Client& client, SDL_Event event) override
  {
    switch (event.type)
    {
      case SDL_EVENT_QUIT:
        client.exit();
        break;
      default:
        break;
    }
  }

 private:
  nib::Renderer renderer;
};

int main()
{
  auto renderer_res = nib::Renderer::new_();
  if (!renderer_res)
  {
    return EXIT_FAILURE;
  }
  Nib nib(renderer_res.unwrap());

  auto client_res = nib::Client::new_(&nib);
  if (!client_res)
  {
    return EXIT_FAILURE;
  }

  auto client = client_res.unwrap();
  client.run();
}
