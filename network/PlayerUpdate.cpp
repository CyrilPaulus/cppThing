#include "PlayerUpdate.h"

PlayerUpdate::PlayerUpdate()
{
  _type = Packet::UpdatePlayer;
  _position = sf::Vector2f(0, 0);
  _eye_position = sf::Vector2f(0, 0);
  _id = 0;
}

sf::Packet PlayerUpdate::encode()
{
  sf::Packet rslt = Packet::encode();
  rslt << _position.x;
  rslt << _position.y;
  rslt << _eye_position.x;
  rslt << _eye_position.y;
  rslt << _id;
  return rslt;
}

void PlayerUpdate::decode(sf::Packet p)
{
  p >> _position.x >> _position.y >> _eye_position.x >> _eye_position.y >> _id;
}

void PlayerUpdate::setId(int id)
{
  _id = (sf::Uint8)id;
}

int PlayerUpdate::getId()
{
  return _id;
}

sf::Vector2f PlayerUpdate::getPosition()
{
  return _position;
}

sf::Vector2f PlayerUpdate::getEyePosition()
{
  return _eye_position;
}

void PlayerUpdate::setPosition(sf::Vector2f position)
{
  _position = position;
}

void PlayerUpdate::setEyePosition(sf::Vector2f eye_position)
{
  _eye_position = eye_position;
}
