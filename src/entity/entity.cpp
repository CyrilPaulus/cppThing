#include "../config.h"
#include "entity.h"

Entity::Entity(ImageManager *imageManager) {
  sprite = new sf::Sprite(*(imageManager->get("cube")));
  position = sf::Vector2f(0, 0);
  bbox = sf::Vector2f(0,0);
  bbox.x = sprite->getGlobalBounds().width;
  bbox.y = sprite->getGlobalBounds().height;
  remove = false;
}

Entity::~Entity() {
  delete sprite;
}

void Entity::draw(sf::RenderTarget *rt) {
  sprite->setPosition(getPosition());
  rt->draw(*sprite);
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
