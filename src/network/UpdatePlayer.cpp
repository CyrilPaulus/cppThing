#include "UpdatePlayer.h"

UpdatePlayer::UpdatePlayer() {
  type = Packet::UpdatePlayer;
  position = sf::Vector2f(0,0);
  eyePosition = sf::Vector2f(0,0);
  id = 0;
}

sf::Packet UpdatePlayer::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << position.x;
  rslt << position.y;
  rslt << eyePosition.x;
  rslt << eyePosition.y;
  rslt << id;
  return rslt;
}


void UpdatePlayer::decode(sf::Packet p) {
  p >> position.x >> position.y >> eyePosition.x >> eyePosition.y >> id;
}

void UpdatePlayer::setId(int id) {
  this->id = (sf::Uint8)id;
}

int UpdatePlayer::getId() {
  return id;
}

sf::Vector2f UpdatePlayer::getPosition() {
  return position;
}

sf::Vector2f UpdatePlayer::getEyePosition() {
  return eyePosition;
}

void UpdatePlayer::setPosition(sf::Vector2f position) {
  this->position = position;
}

void UpdatePlayer::setEyePosition(sf::Vector2f eyePosition) {
  this->eyePosition = position;
}
