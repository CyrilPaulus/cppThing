#include "../config.h"
#include "cubeupdate.h"

CubeUpdate::CubeUpdate(int cubeType, sf::Vector2f position, bool added, int layer){
  this->type = Packet::CubeUpdate;
  this->cubeType = cubeType;
  this->position = position; 
  this->added = added;
  this->layer = layer;
}

/*
void CubeUpdate::Encode(ZCom_BitStream *message){
  message->addInt(Packet::CubeUpdate, 8);
  message->addInt(cubeType, 32);
  message->addFloat(position.x, 23);
  message->addFloat(position.y, 23);
  message->addBool(added);
  message->addInt(layer, 8);
}

CubeUpdate* CubeUpdate::Decode(ZCom_BitStream &message){
  if((int) message.getInt(8) != Packet::CubeUpdate)
   return NULL;
  int cubeType = (int) message.getInt(32);
  float x = message.getFloat(23);
  float y = message.getFloat(23);
  sf::Vector2f position(x, y);
  bool added = message.getBool();
  int layer = message.getInt(8);
  return new CubeUpdate(cubeType, position, added, layer);
}
*/

int CubeUpdate::GetCubeType() {
  return cubeType;
}

sf::Vector2f CubeUpdate::GetPosition() {
  return position;
}

bool CubeUpdate::GetAdded() {
  return added;
}

int CubeUpdate::GetLayer() {
  return layer;
}
