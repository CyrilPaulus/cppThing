#include "ClientConnect.h"

ClientConnect::ClientConnect() {
  type = Packet::ClientConnect;
  r = 0;
  g = 0;
  b = 0;
  id = 0;
}

sf::Packet ClientConnect::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << r;
  rslt << g;
  rslt << b;
  rslt << id;
  return rslt;
}


void ClientConnect::decode(sf::Packet p) {
  p >> r >> g >> b >> id;
}

void ClientConnect::setPseudo(std::string pseudo) {
  this->pseudo = pseudo;
}

void ClientConnect::setColor(sf::Color color) {
  this->r = color.r;
  this->g = color.g;
  this->b = color.b;
}

void ClientConnect::setId(int id) {
  this->id = (sf::Uint8)id;
}

sf::Color ClientConnect::getColor() {
  return sf::Color(r, g ,b);
}

int ClientConnect::getId() {
  return id;
}

std::string ClientConnect::getPseudo() {
  return pseudo;
}
