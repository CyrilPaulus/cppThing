#include "../config.h"
#include "cube.h"

Cube::Cube(int type) : Entity() {
  _bbox.x = Cube::WIDTH;
  _bbox.y = Cube::HEIGHT;
  setType(type);
}

void Cube::setType(int type) {
  _type = type;  
}

int Cube::getType() {
  return _type;
}
