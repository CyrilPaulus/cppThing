#include "../config.h"
#include "entity.h"

Entity::Entity() {

  position = sf::Vector2f(0, 0);
  bbox = sf::Vector2f(0,0);
  remove = false;
}

Entity::~Entity() {
}

void Entity::update(float frametime){
}

sf::FloatRect Entity::getBbox() {
  return sf::FloatRect(position.x, position.y, bbox.x, bbox.y);
}

sf::Vector2f Entity::getPosition() {
  return position;
}

sf::Vector2f Entity::getCenter() {
  sf::Vector2f hbbox = sf::Vector2f(bbox.x / 2, bbox.y / 2);
  return getPosition() + hbbox;
}

void Entity::setPosition(sf::Vector2f position){
  this->position = position;
}

bool Entity::canRemove(){
  return remove;
}
