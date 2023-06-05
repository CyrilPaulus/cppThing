#include "PlayerDelete.h"

PlayerDelete::PlayerDelete()
{
  _type = Packet::DeletePlayer;
  _id = 0;
}

sf::Packet PlayerDelete::encode()
{
  sf::Packet rslt = Packet::encode();
  rslt << _id;
  return rslt;
}

void PlayerDelete::decode(sf::Packet p)
{
  p >> _id;
}

void PlayerDelete::setId(int id)
{
  _id = (sf::Uint8)id;
}

int PlayerDelete::getId()
{
  return _id;
}
