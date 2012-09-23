#include "../config.h"
#include "cube.h"

Cube::Cube(int type) : Entity() {
    this->bbox.x = Cube::WIDTH;
    this->bbox.y = Cube::HEIGHT;
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
}

int Cube::getType() {
  return this->type;
}
