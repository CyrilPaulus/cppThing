#include "NetworkClient.h"

NetworkClient::NetworkClient(unsigned int id, ENetPeer* peer) {
  this->peer = peer;
  this->id = id;
}

unsigned int NetworkClient::getId() {
  return this->id;
}

unsigned int NetworkClient::getIp() {
  return this->peer->address.host;
}

unsigned int NetworkClient::getPort() {
  return this->peer->address.port;
}

ENetPeer* NetworkClient::getPeer() {
  return this->peer;
}

void NetworkClient::setPlayer(Player* p) {
  this->player = p;
}

Player* NetworkClient::getPlayer() {
  return this->player;
}
