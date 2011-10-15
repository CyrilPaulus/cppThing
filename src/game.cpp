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
  ip = "localhost";
  port = 50645;
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
    Client* c = new Client(window, imageManager, zcom);
    c->SetIp(ip);
    c->SetPort(port);
    Cube::RegisterClass(c, false);
    Player::RegisterClass(c, false);
    c->Connect();
	Screen* screens[1];
	int screen = 0;
	screens[0] = c;
	while (screen >= 0) {
		int prevScreen = screen;
		screen = screens[screen]->Run();
	}
    delete c;
}

void Game::RunLocal() {
	ImageManager* serverImgManager = new ImageManager();
	Server* s = new Server(serverImgManager, zcom);
    Cube::RegisterClass(s, true);
    Player::RegisterClass(s, true);
    
    sf::Thread serverThread(&startServer, s);
    
    s->Init();
    serverThread.Launch();
	sf::Sleep(1);
	this->RunClient();
    
    s->Stop();
    serverThread.Wait();    
    delete s;
	delete serverImgManager;
}

void Game::RunServer() {
	Server* s = new Server(imageManager, zcom);
    printf("PORT : %d\n", port);
	s->SetPort(port);
    Cube::RegisterClass(s, true);
    Player::RegisterClass(s, true);
    s->Init();
    s->Run();
    delete s;
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
