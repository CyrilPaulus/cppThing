#include "../config.h"
#include "CubeUpdate.h"

CubeUpdate::CubeUpdate()
{
  _type = Packet::CubeUpdate;
  _cube_type = 0;
  _position = sf::Vector2f(0, 0);
  _added = true;
  _layer = 0;
}

CubeUpdate::CubeUpdate(int cube_type, sf::Vector2f position, bool added, int layer)
{
  _type = Packet::CubeUpdate;
  _cube_type = cube_type;
  _position = position;
  _added = added;
  _layer = layer;
}

sf::Packet CubeUpdate::encode()
{
  sf::Packet rslt = Packet::encode();
  rslt << _cube_type;
  rslt << _position.x;
  rslt << _position.y;
  rslt << _added;
  rslt << _layer;
  return rslt;
}

void CubeUpdate::decode(sf::Packet p)
{
  p >> _cube_type >> _position.x >> _position.y >> _added >> _layer;
}

int CubeUpdate::GetCubeType()
{
  return (int)_cube_type;
}

sf::Vector2f CubeUpdate::GetPosition()
{
  return _position;
}

bool CubeUpdate::GetAdded()
{
  return _added;
}

int CubeUpdate::GetLayer()
{
  return (int)_layer;
}
