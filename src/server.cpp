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
  this->ZCom_processInput();
  this->ZCom_processOutput();
}

//zoidcom handling

bool Server::ZCom_cbConnectionRequest(ZCom_ConnID id, ZCom_BitStream &request, ZCom_BitStream &reply){
  printf("A client requested connection, id : [%d].\n", id);
  return true;
}

void Server::ZCom_cbConnectionSpawned(ZCom_ConnID id) {
  printf("Connection with client [%d] established.\n", id);
}

void Server::ZCom_cbConnectionClosed(ZCom_ConnID id, eZCom_CloseReason reason, ZCom_BitStream &reasondata) {
  printf("Connection with client [%d] closed.\n", id);
}

void Server::Init(){
  this->ZCom_setDebugName("Server");
  if (!this->ZCom_initSockets(true, 50645, 0)){
    printf("No sockets\n");
    exit(255);
  }
  printf("Socket created\n");
}
