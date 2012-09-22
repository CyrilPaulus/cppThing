#include "NetworkClient.h"

NetworkClient::NetworkClient(unsigned int id, unsigned int ip, unsigned int port) {
  this->id = id;
  this->ip = ip;
  this->port = port;
}

unsigned int NetworkClient::getId() {
  return this->id;
}

unsigned int NetworkClient::getIp() {
  return this->ip;
}

unsigned int NetworkClient::getPort() {
  return this->port;
}

void NetworkClient::setPlayer(Player* p) {
  this->player = p;
}

Player* NetworkClient::getPlayer() {
  return this->player;
}
