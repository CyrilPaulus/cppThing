#include "../config.h"
#include "CubeUpdate.h"

CubeUpdate::CubeUpdate() {
  this->type = Packet::CubeUpdate;
  this->cubeType = 0;
  this->position = sf::Vector2f(0,0); 
  this->added = true;
  this->layer = 0;
}

CubeUpdate::CubeUpdate(int cubeType, sf::Vector2f position, bool added, int layer){
  this->type = Packet::CubeUpdate;
  this->cubeType = cubeType;
  this->position = position; 
  this->added = added;
  this->layer = layer;
}

sf::Packet CubeUpdate::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << cubeType;
  rslt << position.x;
  rslt << position.y;
  rslt << added;
  rslt << layer;
  return rslt;
}

void CubeUpdate::decode(sf::Packet p) {
  p  >> cubeType >> position.x >> position.y >> added >> layer; 
}

int CubeUpdate::GetCubeType() {
  return (int)cubeType;
}

sf::Vector2f CubeUpdate::GetPosition() {
  return position;
}

bool CubeUpdate::GetAdded() {
  return added;
}

int CubeUpdate::GetLayer() {
  return (int)layer;
}
