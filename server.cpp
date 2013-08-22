#include "config.h"
#include "server.h"
#include <stdio.h>
#include <stdexcept>
#include <SFML/System.hpp>

#include "network/CubeUpdate.h"
#include "network/UserMessage.h"
#include "network/ClientToken.h"
#include "network/PlayerAdd.h"
#include "network/PlayerDelete.h"
#include "network/PlayerUpdate.h"
#include "network/TextMessage.h"

#include <glog/logging.h>

Server::Server() {
  this->world = new World(true);
  this->running = false;
  this->ticker = new Ticker();
  this->ticker->setUpdateRate(GameConstant::UPDATE_RATE);
  this->port = 50645;
  this->maxClient = 32;
  this->lastClientID = 0;
  this->server = NULL;
}

Server::~Server() {
  delete world;
  delete ticker;
}

void Server::run() {
  running = true;

  if(enet_initialize() != 0) {
    throw std::runtime_error("ENet initialization failed");
  }

  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = this->port;
  server = enet_host_create(&address, this->maxClient, 2, 0, 0);

  if(server == NULL) {
    throw std::runtime_error("ENet server initialization failed");
  }

  LOG(INFO) << "Server started";

  if(!world->load("world.map"))
    world->addCube(sf::Vector2f(0,90), 1, 1);
  
  while(running) {
    //Manage network events
    ENetEvent event;
    while(enet_host_service(server, &event, 0) > 0) {
      switch(event.type) {
      case ENET_EVENT_TYPE_CONNECT:
	addClient(event.peer);
	break;
      case ENET_EVENT_TYPE_RECEIVE:{
	sf::Packet p;
	p.append(event.packet->data, event.packet->dataLength);
	handlePacket(p, event.peer);
	enet_packet_destroy(event.packet);
	break;
      }
      case ENET_EVENT_TYPE_DISCONNECT:
	removeClient(event.peer->address.host, event.peer->address.port);
	break;
      default:
	break;
      }
    }
    
    if(ticker->tick())
      update(ticker->getElapsedTime());
    sf::sleep(sf::seconds(0.01f));
  }
  world->save("world.map");
  enet_host_destroy(server);
  enet_deinitialize();
  LOG(INFO) << "Server finished";
}

void Server::addClient(ENetPeer* peer) {
  NetworkClient* c = new NetworkClient(lastClientID, peer);
  clients.push_back(c);
  LOG(INFO) << "Client connected to server: " << c->getId();
  lastClientID++;
  ClientToken ct;
  ct.setId(c->getId());
  sendReliable(peer, &ct);
}

void Server::removeClient(unsigned int ip, unsigned int port) {
  std::list<NetworkClient*>::iterator it;
  for(it = clients.begin(); it != clients.end(); it++) {
    if((*it)->getIp() == ip && (*it)->getPort() == port) {
      LOG(INFO) << "Client disconnected from server: " << (*it)->getId();
      _client_names.erase((*it)->getPlayer()->getPseudo());
      PlayerDelete dp;
      dp.setId((*it)->getId());
      broadcastReliable(&dp);
      delete(*it);
      clients.erase(it);
      break;
    }
  }
}

void Server::stop() {
  LOG(INFO) << "Closing server...";
  running = false;
}

void Server::update(sf::Time frametime) {
  
  world->updatePlayer(frametime, Input());
}

void Server::handlePacket(sf::Packet p, ENetPeer* peer) {
  sf::Uint8 type;
  p >> type;
  switch(type) {
  case Packet::CubeUpdate:{
    CubeUpdate cu;
    cu.decode(p);
    if(cu.GetAdded() && world->canAddCube(cu.GetPosition(), cu.GetLayer())) {
      world->addCube(cu.GetPosition(), cu.GetCubeType(), cu.GetLayer());
      broadcastReliable(&cu);
    }
    else if(!cu.GetAdded() && world->canRemoveCube(cu.GetPosition(), cu.GetLayer())) {
      world->removeCube(cu.GetPosition(), cu.GetLayer());
      broadcastReliable(&cu);
    }
    break;
  }
  case Packet::UserMessage:{
    break;
  }
  //Used to validate client connection
  case Packet::AddPlayer: {
      PlayerAdd pa;
      pa.decode(p);
      NetworkClient *c = getClientByPeer(peer);
      Player* p = new Player(world);
      p->setColor(pa.getColor());
      p->setPseudo(getUniquePseudo(pa.getPseudo()));
      p->setId(pa.getId());
      c->setPlayer(p);
      sendFullWorldUpdate(peer);
      pa.setPseudo(p->getPseudo());
      broadcastReliable(&pa);
    break;
  }
  case Packet::UpdatePlayer:{
    PlayerUpdate up;
    up.decode(p);
    broadcast(&up);
    break;
  }
  case Packet::TextMessage:{
    NetworkClient *c = getClientByPeer(peer);
    TextMessage tm;
    tm.decode(p);
    tm.setMessage(c->getPlayer()->getPseudo() + ": " + tm.getMessage());
    LOG(INFO) << "MSG " << tm.getMessage();
    broadcast(&tm);
    break;
  }
  default: 
    break;
  }
}

void Server::init(){
}

void Server::setPort(int port){
  this->port = port;
}

void Server::broadcastReliable(Packet *p) {
  sf::Packet data = p->encode();
  ENetPacket* packet = enet_packet_create(data.getData(), data.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_host_broadcast(server, 0, packet);
}

void Server::broadcast(Packet *p) {
  sf::Packet data = p->encode();
  ENetPacket* packet = enet_packet_create(data.getData(), data.getDataSize(), 0);
  enet_host_broadcast(server, 1, packet);
}

void Server::sendReliable(ENetPeer* peer, Packet *p) {
  sf::Packet data = p->encode();
  ENetPacket* packet = enet_packet_create(data.getData(), data.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(peer, 0, packet);
}

void Server::sendFullWorldUpdate(ENetPeer* peer) {
  for(int i = 0; i < GameConstant::LAYERNBR; i++) {
    std::list<Cube*> layer = world->getList(i);
    std::list<Cube*>::iterator it;
    for(it = layer.begin(); it != layer.end(); it++) {
      CubeUpdate cu((*it)->getType(), (*it)->getPosition(), true, i);
      sendReliable(peer, &cu);
    }
  }

  NetworkClient *c = getClientByPeer(peer);
  std::list<NetworkClient*>::iterator it;
  for(it = clients.begin(); it != clients.end(); it++) {
    if((*it)->getPlayer() != NULL && (*it) != c) {
      PlayerAdd ap;
      ap.setColor((*it)->getPlayer()->getColor());
      ap.setId((*it)->getId());
      ap.setPseudo((*it)->getPlayer()->getPseudo());
      sendReliable(peer, &ap);
    }
  }
}

NetworkClient* Server::getClientByPeer(ENetPeer* peer) {
  unsigned int ip = peer->address.host;
  unsigned int port = peer->address.port;
  std::list<NetworkClient*>::iterator it;
  for(it = clients.begin(); it != clients.end(); it++) {
    if((*it)->getIp() == ip && (*it)->getPort() == port) {
      return (*it);
      break;
    }
  }
  return NULL;
}

std::string Server::getUniquePseudo(std::string pseudo) {
  if(pseudo.compare("") == 0)
    pseudo = "Anon";
  
  int i = 0;
  std::string candidate = pseudo;
  while(_client_names.find(candidate) != _client_names.end()) {;
    std::stringstream ss;
    ss << pseudo << " (" << i << ")";
    candidate = ss.str();
    i++;
  }
  _client_names.insert(candidate);
  return candidate;
}
