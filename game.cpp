#include "config.h"
#include "game.h"
#include <time.h>
#include <SFML/System.hpp>
#include <stdio.h>

#include "client.h"
#include "server.h"
#include "entity/cube.h"
#include "menu/mainMenu.h"
#include "menu/connectMenu.h"
#include "menu/optionMenu.h"

#include <map>

void startServerThread(void* server);

Game::Game(){
  _image_manager = new ImageManager();
  _ip = "localhost";
  _port = 50645;
  _server_running = false;
  _s = NULL;
  _c = NULL;
  _server_thread = NULL;
  _type = Game::LOCAL;
}

Game::~Game(){
  if(_window)
    delete _window;
  delete _image_manager;  
}

void Game::run(int type){
  _type = type;
  srand(time(NULL));
  if(type == Game::LOCAL){
    runLocal();       
  }
  else if(type == Game::CLIENT) {
    runClient();
  }
  else if(type == Game::SERVER) {
    runServer();
  }

}

int Game::getType() {
  return _type;
}

void Game::runClient() {
  _window = new sf::RenderWindow(sf::VideoMode(800,600), "2dThing c++");
  _window->setFramerateLimit(GameConstant::FRAMERATE_LIMIT);
  _window->setMouseCursorVisible(false);
  _c = new Client(_window, _image_manager);
  _c->setIp(_ip);
  _c->setPort(_port);
  _c->setPseudo(_pseudo);

  _c->connect();
  _c->StartThread();

  MainMenu* main = new MainMenu(_window, _image_manager, this);
  ConnectMenu* connect = new ConnectMenu(_window, _image_manager, this);
  OptionMenu* option = new OptionMenu(_window, _image_manager, this);

  int screen = 1;
  screens[Screen::GAME] = _c;
  screens[Screen::MAINMENU] = main;
  screens[Screen::CONNECT] = connect;
  screens[Screen::OPTION] = option;
  while (screen != Screen::EXIT) {
    screen = screens[screen]->run();
  }
  _c->StopThread();
  delete _c;
  delete main;
}

void Game::runLocal() {
  startServer();
  runClient();
  stopServer();
}

void Game::runServer() {
  startServer();
  scanf("\n");
  stopServer();
}

void Game::startServer() {
  _s = new Server();
 
  _server_thread = new sf::Thread(&startServerThread, _s);
  
  _s->setPort(_port);
  _s->init();
  
  _server_thread->launch();
  _server_running = true;
}

void Game::stopServer() {
  if(_server_running) {
    _s->stop();
    _server_thread->wait();    
    delete _server_thread;
    delete _s;
    _server_running = false;
  }
}

void startServerThread(void* server){
  (static_cast<Server*>(server))->run();
}

void Game::setIp(std::string ip){
  _ip = ip;
}

void Game::setPort(int port){
  _port = port;
}

void Game::connect(std::string ip) {
  //First try to split ip and port
  size_t found = ip.find(":");
  if(found != std::string::npos) {
    setIp(ip.substr(0, found));
    std::cout << _ip << std::endl;
    int port = atoi(ip.substr(found+1, ip.size() - found).c_str());
    setPort(port);    
  } else {
    setIp(ip);
    setPort(50645);
  }

  if(_c != NULL) {
    _c->StopThread();
    delete _c;
    _c = NULL;
  }

  _c = new Client(_window, _image_manager);
  _c->setIp(_ip);
  _c->setPort(_port);
  _c->setPseudo(_pseudo);

  screens[Screen::GAME] = _c;  
  _c->connect();
  _c->StartThread();


}

Client* Game::getClient() {
  return _c;
}

Server* Game::getServer() {
  return _s;
}

void Game::setPseudo(std::string p) {
  _pseudo = p;
}

std::string Game::getIpStr() {
  std::stringstream ss;
  ss << _ip << ":" << _port;
  return ss.str();
}

std::string Game::getPseudo() {
  return _c->getPseudo();
}

sf::Vector3i Game::getColor() {
  return _c->getColor();
}

void Game::UpdatePlayerInfo(std::string name, sf::Vector3i color) {
  _c->UpdatePlayer(name, color);
}

