#include "config.h"
#include "server.h"
#include <stdio.h>
#include <SFML/System.hpp>

Server::Server(ImageManager* imageManager) {
  this->imageManager = imageManager;
  this->world = new World(imageManager);
  this->running = false;
  this->ticker = new Ticker();
}

Server::~Server() {
  delete world;
  delete ticker;
}

void Server::Run() {
  running = true;
  printf("Server started\n");
  while(running) {
    if(ticker->Tick())
      Update(ticker->GetElapsedSeconds());
    else
      sf::Sleep(0.01f);
  }
  printf("Server finished\n");
}

void Server::Stop() {
  printf("Closing server...\n");
  running = false;
}

void Server::Update(float frametime) {
  
}
