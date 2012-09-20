#include "config.h"
#include "server.h"
#include <stdio.h>
#include <SFML/System.hpp>
#include "network/cubeupdate.h"
#include "network/usermessage.h"

Server::Server(ImageManager* imageManager) {
  this->imageManager = imageManager;
  this->world = new World(imageManager, true);
  this->running = false;
  this->ticker = new Ticker();
  this->ticker->SetUpdateRate(GameConstant::UPDATE_RATE);
  this->port = 50645;
}

Server::~Server() {
  delete world;
  delete ticker;
}

void Server::Run() {
  running = true;
  printf("Server started\n");
  world->AddCube(sf::Vector2f(0,90), 1, 1);
  while(running) {
    if(ticker->Tick())
      Update(ticker->GetElapsedMilliSeconds());
    else {
      sf::sleep(sf::seconds(0.01f));
    }
  }
  printf("Server finished\n");
}

void Server::Stop() {
  printf("Closing server...\n");
  running = false;
}

void Server::Update(unsigned int frametime) {
  world->UpdatePlayer((GameConstant::SIMULATION_TIME_PER_UPDATE), Input());
}

//zoidcom handling

/*
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
      world->AddCube(cu->GetPosition(), cu->GetCubeType(), cu->GetLayer());
    else
      world->RemoveCube(cu->GetPosition(), cu->GetLayer());
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
*/

void Server::Init(){
  //TODO create TCP socket and other things
  printf("Socket created\n");
}

void Server::SetPort(int port){
  this->port = port;
}
