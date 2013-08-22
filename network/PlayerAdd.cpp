#include "PlayerAdd.h"

PlayerAdd::PlayerAdd() {
  _type = Packet::AddPlayer;
  _r = 0;
  _g = 0;
  _b = 0;
  _id = 0;
}

sf::Packet PlayerAdd::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << _r;
  rslt << _g;
  rslt << _b;
  rslt << _id;
  rslt << _pseudo;
  return rslt;
}


void PlayerAdd::decode(sf::Packet p) {
  p >> _r >> _g >> _b >> _id >> _pseudo;
}

void PlayerAdd::setPseudo(std::string pseudo) {
  _pseudo = pseudo;
}

void PlayerAdd::setColor(sf::Vector3i color) {
  _r = color.x;
  _g = color.y;
  _b = color.z;
}

void PlayerAdd::setId(int id) {
  _id = (sf::Uint8)id;
}

sf::Vector3i PlayerAdd::getColor() {
  return sf::Vector3i(_r, _g ,_b);
}

int PlayerAdd::getId() {
  return _id;
}

std::string PlayerAdd::getPseudo() {
  return _pseudo;
}
