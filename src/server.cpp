#include "config.h"
#include "server.h"
#include <stdio.h>
#include <SFML/System.hpp>
#include "network/cubeupdate.h"

Server::Server(ImageManager* imageManager) {
  this->imageManager = imageManager;
  this->world = new World(this, imageManager, true);
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
  std::string pseudo(request.getStringStatic());
  printf("Pseudo : %s", pseudo.data());
  //TODO Validate pseudo
  reply.addInt(id, 32);
  reply.addString(pseudo.data());
  return true;
}

void Server::ZCom_cbConnectionSpawned(ZCom_ConnID id) {
  printf("[SERVER] Connection with client [%d] established.\n", id);
  Player *p = new Player(imageManager, world);
  p->SetID(id);
  p->RegisterZCom(this, true);
  world->AddPlayer(p);
}

void Server::ZCom_cbConnectionClosed(ZCom_ConnID id, eZCom_CloseReason reason, ZCom_BitStream &reasondata) {
  printf("[SERVER] Connection with client [%d] closed.\n", id);
  world->RemovePlayerByID(id);
}

void Server::ZCom_cbDataReceived( ZCom_ConnID id, ZCom_BitStream &data ){
  ZCom_BitStream::BitPos position;
  data.saveReadState(position);
  int type = data.getInt(8);
  data.restoreReadState(position);
  switch(type){
  case Packet::CubeUpdate:{
    CubeUpdate* cu = CubeUpdate::Decode(data);
    if(cu->GetAdded())
      world->AddCube(cu->GetPosition(), cu->GetCubeType());
    else
      world->RemoveCube(cu->GetPosition());
    delete cu;
    break;
  }
  default:
    break;
  }
}

bool Server::ZCom_cbZoidRequest( ZCom_ConnID id, zU8 requested_level, ZCom_BitStream &reason ) {

  return requested_level == 1;
}

void Server::Init(){
  this->ZCom_setDebugName("Server");
  if (!this->ZCom_initSockets(true, 50645, 0)){
    printf("No sockets\n");
    exit(255);
  }
  printf("Socket created\n");
}
