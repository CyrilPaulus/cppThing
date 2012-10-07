#include "PlayerAdd.h"

PlayerAdd::PlayerAdd() {
  type = Packet::AddPlayer;
  r = 0;
  g = 0;
  b = 0;
  id = 0;
}

sf::Packet PlayerAdd::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << r;
  rslt << g;
  rslt << b;
  rslt << id;
  return rslt;
}


void PlayerAdd::decode(sf::Packet p) {
  p >> r >> g >> b >> id;
}

void PlayerAdd::setPseudo(std::string pseudo) {
  this->pseudo = pseudo;
}

void PlayerAdd::setColor(sf::Vector3i color) {
  this->r = color.x;
  this->g = color.y;
  this->b = color.z;
}

void PlayerAdd::setId(int id) {
  this->id = (sf::Uint8)id;
}

sf::Vector3i PlayerAdd::getColor() {
  return sf::Vector3i(r, g ,b);
}

int PlayerAdd::getId() {
  return id;
}

std::string PlayerAdd::getPseudo() {
  return pseudo;
}
