#include "../config.h"
#include "entity.h"

Entity::Entity(ImageManager *imageManager) {
  sf::Image *image = imageManager->get("cube");
  sprite = new sf::Sprite(*(image));
  offset = sf::Vector2f(0, 0);
  position = sf::Vector2f(0, 0);
  bbox = sprite->GetSize();
}

Entity::~Entity() {
  delete sprite;
}

void Entity::Draw(sf::RenderTarget *rt) {
  sprite->SetPosition(position);
  rt->Draw(*sprite);
}

void Entity::Update(float frametime){
}

sf::FloatRect Entity::GetBbox() {
  return sf::FloatRect( position.x, position.y, bbox.x, bbox.y);
}

sf::Vector2f Entity::GetPosition() {
  return position;
}

void Entity::SetPosition(sf::Vector2f position){
  this->position = position;
}
