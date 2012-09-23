#include "../config.h"
#include "cube.h"

Cube::Cube(ImageManager* imageManager, int type) : Entity(imageManager) {
  this->imageManager = imageManager;
  sprite->setTexture(*(imageManager->get("tileset")), true);
  setType(type);
}

/*
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
*/
void Cube::setType(int type) {
  this->type = type;
  int x = (type % 16) * Cube::WIDTH;
  int y = (type / 16) * Cube::HEIGHT;
  sprite->setTextureRect(sf::IntRect(x, y, Cube::WIDTH, Cube::HEIGHT));
}

int Cube::getType() {
  return this->type;
}
