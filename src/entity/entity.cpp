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

void Entity::Draw(sf::RenderTarget *rt) {
  sprite->setPosition(GetPosition());
  rt->draw(*sprite);
}

void Entity::Update(float frametime){
}

sf::FloatRect Entity::GetBbox() {
  return sf::FloatRect(position.x, position.y, bbox.x, bbox.y);
}

sf::Vector2f Entity::GetPosition() {
  return position;
}

sf::Vector2f Entity::GetCenter() {
  sf::Vector2f hbbox = sf::Vector2f(bbox.x / 2, bbox.y / 2);
  return GetPosition() + hbbox;
}

void Entity::SetPosition(sf::Vector2f position){
  this->position = position;
}

bool Entity::CanRemove(){
  return remove;
}
