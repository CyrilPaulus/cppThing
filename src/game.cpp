#include "config.h"
#include "game.h"

#include <time.h>
#include <SFML/System.hpp>

#include "client.h"
#include "server.h"

void startServer(void* server);

Game::Game(){
  window = new sf::RenderWindow(sf::VideoMode(800,600), "2dThing c++");
  window->SetFramerateLimit(GameConstant::FRAMERATE_LIMIT);
  window->ShowMouseCursor(false);
  imageManager = new ImageManager();
}

Game::~Game(){
  delete window;
  delete imageManager;  
}

void Game::Run(){
  srand(time(NULL));
  Server s(imageManager);
  Client c(window, imageManager);
  sf::Thread serverThread(&startServer, &s);
  serverThread.Launch();
  c.Run();
  s.Stop();
  serverThread.Wait();
}

void startServer(void* server){
  (static_cast<Server*>(server))->Run();
}
