#include "config.h"
#include "game.h"
#include <zoidcom.h>
#include <time.h>
#include <SFML/System.hpp>
#include <stdio.h>

#include "client.h"
#include "server.h"

void startServer(void* server);
void processZoidcomLog(const char *log);

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
  ZoidCom *zcom = new ZoidCom(processZoidcomLog);
  if(!zcom || !zcom->Init())
    exit(255);

  srand(time(NULL));
  Server* s = new Server(imageManager);
  Client* c = new Client(window, imageManager);
  sf::Thread serverThread(&startServer, s);
  
  s->Init();
  serverThread.Launch();
  c->Connect();
  s->Stop();
  serverThread.Wait();
  delete c;
  delete s;
  delete zcom;
}

void startServer(void* server){
  (static_cast<Server*>(server))->Run();
}

void processZoidcomLog(const char *log) {
  printf("%s\n", log);
}
