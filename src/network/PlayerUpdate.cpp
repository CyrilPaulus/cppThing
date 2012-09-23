#include "PlayerUpdate.h"

PlayerUpdate::PlayerUpdate() {
  type = Packet::UpdatePlayer;
  position = sf::Vector2f(0,0);
  eyePosition = sf::Vector2f(0,0);
  id = 0;
}

sf::Packet PlayerUpdate::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << position.x;
  rslt << position.y;
  rslt << eyePosition.x;
  rslt << eyePosition.y;
  rslt << id;
  return rslt;
}


void PlayerUpdate::decode(sf::Packet p) {
  p >> position.x >> position.y >> eyePosition.x >> eyePosition.y >> id;
}

void PlayerUpdate::setId(int id) {
  this->id = (sf::Uint8)id;
}

int PlayerUpdate::getId() {
  return id;
}

sf::Vector2f PlayerUpdate::getPosition() {
  return position;
}

sf::Vector2f PlayerUpdate::getEyePosition() {
  return eyePosition;
}

void PlayerUpdate::setPosition(sf::Vector2f position) {
  this->position = position;
}

void PlayerUpdate::setEyePosition(sf::Vector2f eyePosition) {
  this->eyePosition = eyePosition;
}
