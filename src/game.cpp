#include "config.h"
#include "game.h"
#include <zoidcom.h>
#include <time.h>
#include <SFML/System.hpp>
#include <stdio.h>

#include "client.h"
#include "server.h"
#include "entity/cube.h"

void startServer(void* server);
void processZoidcomLog(const char *log);

Game::Game(){
  imageManager = new ImageManager();
}

Game::~Game(){
  if(window)
    delete window;
  delete imageManager;  
}

void Game::Run(int type){
  ZoidCom *zcom = new ZoidCom(processZoidcomLog);
  if(!zcom || !zcom->Init())
    exit(255);

  srand(time(NULL));
  if(type == Game::LOCAL){
    window = new sf::RenderWindow(sf::VideoMode(800,600), "2dThing c++");
    window->SetFramerateLimit(GameConstant::FRAMERATE_LIMIT);
    window->ShowMouseCursor(false);
    Server* s = new Server(imageManager);
    Cube::RegisterClass(s, true);
    Player::RegisterClass(s, true);
    Client* c = new Client(window, imageManager);
    Cube::RegisterClass(c, false);
    Player::RegisterClass(c, false);
    sf::Thread serverThread(&startServer, s);
    
    s->Init();
    serverThread.Launch();
    c->Connect();
    s->Stop();
    serverThread.Wait();
    delete c;
    delete s;    
  }
  else if(type == Game::CLIENT) {
    window = new sf::RenderWindow(sf::VideoMode(800,600), "2dThing c++");
    window->SetFramerateLimit(GameConstant::FRAMERATE_LIMIT);
    window->ShowMouseCursor(false);
    Client* c = new Client(window, imageManager);
    Cube::RegisterClass(c, false);
    Player::RegisterClass(c, false);
    c->Connect();
    delete c;
  }
  else if(type == Game::SERVER) {
    Server* s = new Server(imageManager);
    Cube::RegisterClass(s, true);
    Player::RegisterClass(s, true);
    s->Init();
    s->Run();
    delete s;
  }

  delete zcom;
}

void startServer(void* server){
  (static_cast<Server*>(server))->Run();
}

void processZoidcomLog(const char *log) {
  printf("%s\n", log);
}
