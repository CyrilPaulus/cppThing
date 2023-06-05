#include "UpdatePlayerInfo.h"

UpdatePlayerInfo::UpdatePlayerInfo()
{
  _type = Packet::UpdatePlayerInfo;
  _r = 0;
  _g = 0;
  _b = 0;
  _id = 0;
}

sf::Packet UpdatePlayerInfo::encode()
{
  sf::Packet rslt = Packet::encode();
  rslt << _r;
  rslt << _g;
  rslt << _b;
  rslt << _id;
  rslt << _pseudo;
  return rslt;
}

void UpdatePlayerInfo::decode(sf::Packet p)
{
  p >> _r >> _g >> _b >> _id >> _pseudo;
}

void UpdatePlayerInfo::setPseudo(std::string pseudo)
{
  _pseudo = pseudo;
}

void UpdatePlayerInfo::setColor(sf::Vector3i color)
{
  _r = color.x;
  _g = color.y;
  _b = color.z;
}

void UpdatePlayerInfo::setId(int id)
{
  _id = (sf::Uint8)id;
}

sf::Vector3i UpdatePlayerInfo::getColor()
{
  return sf::Vector3i(_r, _g, _b);
}

int UpdatePlayerInfo::getId()
{
  return _id;
}

std::string UpdatePlayerInfo::getPseudo()
{
  return _pseudo;
}
