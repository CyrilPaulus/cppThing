#include "config.h"
#include "game.h"
#include <time.h>
#include <SFML/System.hpp>
#include <stdio.h>

#include "client.h"
#include "server.h"
#include "entity/cube.h"
#include "menu/mainMenu.h"

void startServerThread(void* server);

Game::Game(){
  imageManager = new ImageManager();
  ip = "localhost";
  port = 50645;
  serverRunning = false;
  s = NULL;
  c = NULL;
  serverThread = NULL;
}

Game::~Game(){
  if(window)
    delete window;
  delete imageManager;  
}

void Game::run(int type){
  
  srand(time(NULL));
  if(type == Game::LOCAL){
    this->runLocal();       
  }
  else if(type == Game::CLIENT) {
    this->runClient();
  }
  else if(type == Game::SERVER) {
    this->runServer();
  }

}

void Game::runClient() {
  window = new sf::RenderWindow(sf::VideoMode(800,600), "2dThing c++");
  window->setFramerateLimit(GameConstant::FRAMERATE_LIMIT);
  window->setMouseCursorVisible(false);
  c = new Client(window, imageManager);
  c->setIp(ip);
  c->setPort(port);
  
  c->connect();

  MainMenu* main = new MainMenu(window, imageManager, this);
  Screen* screens[6];
  int screen = 1;
  screens[Screen::GAME] = c;
  screens[Screen::MAINMENU] = main;
  while (screen != Screen::EXIT) {
    screen = screens[screen]->run();
  }
  delete c;
  delete main;
}

void Game::runLocal() {
  this->startServer();
  this->runClient();
  this->stopServer();
}

void Game::runServer() {
  this->startServer();
  scanf("\n");
  this->stopServer();
}

void Game::startServer() {
  serverImgManager = new ImageManager();
  s = new Server(serverImgManager);
 
  serverThread = new sf::Thread(&startServerThread, s);
  
  s->init();
  serverThread->launch();
  serverRunning = true;
}

void Game::stopServer() {
  if(serverRunning) {
    s->stop();
    serverThread->wait();    
    delete serverThread;
    delete s;
    delete serverImgManager;
    serverRunning = false;
  }
}

void startServerThread(void* server){
  (static_cast<Server*>(server))->run();
}

void Game::setIp(std::string ip){
  this->ip = ip;
}

void Game::setPort(int port){
  this->port = port;
}

Client* Game::getClient() {
  return c;
}

Server* Game::getServer() {
  return s;
}
