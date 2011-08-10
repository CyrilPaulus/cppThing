#include "../config.h"
#include "cube.h"


Cube::Cube(ImageManager* imageManager, int type) : Entity(imageManager) {
  this->imageManager = imageManager;
  sprite->SetTexture(*(imageManager->get("tileset")), true);
  SetType(type);
}

void Cube::RegisterZCom(ZCom_Control *control, bool server) {
  node->registerNodeDynamic(GetClass(server), control);
  if(server){
    ZCom_BitStream *adata = new ZCom_BitStream();
    adata->addInt(type, 32);
    adata->addFloat(position.x, 23);
    adata->addFloat(position.y, 23);
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
    netClassServerId = control->ZCom_registerClass("cube", ZCOM_CLASSFLAG_ANNOUNCEDATA);
  else
    netClassClientId = control->ZCom_registerClass("cube", ZCOM_CLASSFLAG_ANNOUNCEDATA);
}
