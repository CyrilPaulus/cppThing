#include "../config.h"
#include "cube.h"

ZCom_ClassID Cube::netClassServerId = ZCom_Invalid_ID;
ZCom_ClassID Cube::netClassClientId = ZCom_Invalid_ID;

Cube::Cube(ImageManager* imageManager, int type) : Entity(imageManager) {
  this->imageManager = imageManager;
  sprite->SetTexture(*(imageManager->get("tileset")), true);
  SetType(type);
}

void Cube::RegisterZCom(ZCom_Control *control, bool server, int layer) {
  node->registerNodeDynamic(GetClass(server), control);
  if(server){
    ZCom_BitStream *adata = new ZCom_BitStream();
    adata->addInt(type, 32);
    adata->addFloat(position[0], 23);
    adata->addFloat(position[1], 23);
    adata->addInt(layer, 8);
    node->setAnnounceData(adata);
  }
}

void Cube::SetType(int type) {
  this->type = type;
  int x = (type % 16) * Cube::WIDTH;
  int y = (type / 16) * Cube::HEIGHT;
  sprite->SetSubRect(sf::IntRect(x, y, Cube::WIDTH, Cube::HEIGHT));
}

void Cube::RegisterClass(ZCom_Control * control, bool server){
  if(server)
    Cube::netClassServerId = control->ZCom_registerClass("cube", ZCOM_CLASSFLAG_ANNOUNCEDATA);
  else
    Cube::netClassClientId = control->ZCom_registerClass("cube", ZCOM_CLASSFLAG_ANNOUNCEDATA);
}


ZCom_ClassID Cube::GetClass(bool server) {
  if(server)
    return(Cube::netClassServerId);
  else
    return(Cube::netClassClientId);
}
