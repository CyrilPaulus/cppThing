#include "config.h"
#include "game.h"
#include <zoidcom.h>
#include <time.h>
#include <SFML/System.hpp>
#include <stdio.h>

#include "client.h"
#include "server.h"
#include "entity/cube.h"
#include "menu/mainMenu.h"

void startServer(void* server);
void processZoidcomLog(const char *log);

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

void Game::Run(int type){
  zcom = new ZoidCom(processZoidcomLog);
  if(!zcom || !zcom->Init())
    exit(255);

  srand(time(NULL));
  if(type == Game::LOCAL){
    this->RunLocal();       
  }
  else if(type == Game::CLIENT) {
    this->RunClient();
  }
  else if(type == Game::SERVER) {
    this->RunServer();
  }

  delete zcom;
}

void Game::RunClient() {
  window = new sf::RenderWindow(sf::VideoMode(800,600), "2dThing c++");
  window->SetFramerateLimit(GameConstant::FRAMERATE_LIMIT);
  window->ShowMouseCursor(false);
  c = new Client(window, imageManager, zcom);
  c->SetIp(ip);
  c->SetPort(port);
  Cube::RegisterClass(c, false);
  Player::RegisterClass(c, false);
  c->Connect();

  MainMenu* main = new MainMenu(window, imageManager, this);
  Screen* screens[6];
  int screen = 1;
  screens[Screen::GAME] = c;
  screens[Screen::MAINMENU] = main;
  while (screen != Screen::EXIT) {
    int prevScreen = screen;
    screen = screens[screen]->Run();
  }
  delete c;
  delete main;
}

void Game::RunLocal() {
  this->StartServer();
  this->RunClient();
  this->StopServer();
}

void Game::RunServer() {
  this->StartServer();
  scanf("\n");
  this->StopServer();
}

void Game::StartServer() {
  serverImgManager = new ImageManager();
  s = new Server(serverImgManager, zcom);
  Cube::RegisterClass(s, true);
  Player::RegisterClass(s, true);
  
  serverThread = new sf::Thread(&startServer, s);
  
  s->Init();
  serverThread->Launch();
  serverRunning = true;
}

void Game::StopServer() {
  if(serverRunning) {
    s->Stop();
    serverThread->Wait();    
    delete serverThread;
    delete s;
    delete serverImgManager;
    serverRunning = false;
  }
}

void startServer(void* server){
  (static_cast<Server*>(server))->Run();
}

void processZoidcomLog(const char *log) {
  printf("%s\n", log);
}

void Game::SetIp(std::string ip){
  this->ip = ip;
}

void Game::SetPort(int port){
  this->port = port;
}

Client* Game::GetClient() {
  return c;
}

Server* Game::GetServer() {
  return s;
}
