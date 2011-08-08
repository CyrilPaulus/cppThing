#include "../config.h"
#include "cube.h"


Cube::Cube(int type, ImageManager* imageManager) : Entity(imageManager) {
  this->imageManager = imageManager;
  sprite->SetImage(*(imageManager->get("tileset")), true);
  SetType(type);
}

void Cube::SetType(int type) {
  this->type = type;
  int x = (type % 16) * Cube::WIDTH;
  int y = (type / 16) * Cube::HEIGHT;
  sprite->SetSubRect(sf::IntRect(x, y, Cube::WIDTH, Cube::HEIGHT));
}

