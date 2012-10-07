#include "PlayerDelete.h"

PlayerDelete::PlayerDelete() {
  type = Packet::DeletePlayer;
  id = 0;
}

sf::Packet PlayerDelete::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << id;
  return rslt;
}

void PlayerDelete::decode(sf::Packet p) {
  p >> id;
}

void PlayerDelete::setId(int id) {
  this->id = (sf::Uint8)id;
}

int PlayerDelete::getId() {
  return id;
}

