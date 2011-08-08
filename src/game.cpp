#include "config.h"
#include "game.h"
#include <time.h>

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
  Client c(window, imageManager);
  c.Run();
}
