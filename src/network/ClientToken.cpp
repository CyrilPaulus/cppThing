#include "ClientToken.h"

ClientToken::ClientToken() {
  type = Packet::ClientConnect;
  id = 0;
}

sf::Packet ClientToken::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << id;
  return rslt;
}

void ClientToken::decode(sf::Packet p) {
  p >> id;
}

void ClientToken::setId(int id) {
  this->id = (sf::Uint8)id;
}

int ClientToken::getId() {
  return id;
}

