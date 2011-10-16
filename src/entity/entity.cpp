#include "../config.h"
#include "entity.h"

Entity::Entity(ImageManager *imageManager) {
  sprite = new sf::Sprite(*(imageManager->get("cube")));
  offset = sf::Vector2f(0, 0);
  position = sf::Vector2f(0, 0);
  bbox = sprite->GetSize();
  remove = false;
}

Entity::~Entity() {
  delete sprite;
}

void Entity::Draw(sf::RenderTarget *rt) {
  sprite->SetPosition(GetPosition());
  rt->Draw(*sprite);
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
