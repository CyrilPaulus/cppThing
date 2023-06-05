#include "ClientToken.h"

ClientToken::ClientToken()
{
  _type = Packet::ClientConnect;
  _id = 0;
}

sf::Packet ClientToken::encode()
{
  sf::Packet rslt = Packet::encode();
  rslt << _id;
  return rslt;
}

void ClientToken::decode(sf::Packet p)
{
  p >> _id;
}

void ClientToken::setId(int id)
{
  _id = (sf::Uint8)id;
}

int ClientToken::getId()
{
  return _id;
}
