#include "config.h"
#include "server.h"
#include <stdio.h>
#include <SFML/System.hpp>
#include "network/cubeupdate.h"
#include "network/usermessage.h"

Server::Server(ImageManager* imageManager, ZoidCom* zcom) {
  this->imageManager = imageManager;
  this->world = new World(this, imageManager, true);
  this->running = false;
  this->ticker = new Ticker();
  this->zcom = zcom;
}

Server::~Server() {
  delete world;
  delete ticker;
}

void Server::Run() {
  running = true;
  printf("Server started\n");
  world->AddCube(sf::Vector2f(0,90), 1);
  while(running) {
    if(ticker->Tick())
      Update(ticker->GetElapsedMilliSeconds());
    else
      {
      sf::Sleep(0.01f);
      if(zcom)
	zcom->Sleep(10);
    }
  }
  printf("Server finished\n");
}

void Server::Stop() {
  printf("Closing server...\n");
  running = false;
}

void Server::Update(unsigned int frametime) {
  ZCom_processReplicators((GameConstant::SIMULATION_TIME_PER_UPDATE));
  this->ZCom_processInput();
  world->UpdatePlayer(frametime, Input());
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
  p->node->setOwner(id, true);
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
  case Packet::UserMessage:{
    UserMessage* um = UserMessage::Decode(data);
    world->GetPlayerByID(id)->Update(um->GetFrametime(), um->GetInput());
    world->GetPlayerByID(id)->SetEyesPosition(um->GetLookDir());
    delete um;
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
