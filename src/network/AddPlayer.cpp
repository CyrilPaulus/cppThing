#include "AddPlayer.h"

AddPlayer::AddPlayer() {
  type = Packet::AddPlayer;
  r = 0;
  g = 0;
  b = 0;
  id = 0;
}

sf::Packet AddPlayer::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << r;
  rslt << g;
  rslt << b;
  rslt << id;
  return rslt;
}


void AddPlayer::decode(sf::Packet p) {
  p >> r >> g >> b >> id;
}

void AddPlayer::setPseudo(std::string pseudo) {
  this->pseudo = pseudo;
}

void AddPlayer::setColor(sf::Color color) {
  this->r = color.r;
  this->g = color.g;
  this->b = color.b;
}

void AddPlayer::setId(int id) {
  this->id = (sf::Uint8)id;
}

sf::Color AddPlayer::getColor() {
  return sf::Color(r, g ,b);
}

int AddPlayer::getId() {
  return id;
}

std::string AddPlayer::getPseudo() {
  return pseudo;
}
