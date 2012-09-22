#include "DeletePlayer.h"

DeletePlayer::DeletePlayer() {
  type = Packet::DeletePlayer;
  id = 0;
}

sf::Packet DeletePlayer::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << id;
  return rslt;
}

void DeletePlayer::decode(sf::Packet p) {
  p >> id;
}

void DeletePlayer::setId(int id) {
  this->id = (sf::Uint8)id;
}

int DeletePlayer::getId() {
  return id;
}

