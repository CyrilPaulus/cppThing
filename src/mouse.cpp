#include "config.h"
#include "mouse.h"

Mouse::Mouse(sf::RenderWindow *window, sf::RenderTarget *world, ImageManager *imageManager) {
  this->window = window;
  this->world = world;
  this->imageManager = imageManager;
  this->sprite = new sf::Sprite(*(imageManager->get("mouse")));
}

Mouse::~Mouse() {
  delete sprite;
}

sf::Vector2f Mouse::GetPosition() {
  return sprite->GetPosition();
}

sf::Vector2f Mouse::GetWorldPosition() {
  return world->ConvertCoords((unsigned int) sprite->GetPosition().x, (unsigned int) sprite->GetPosition().y);
}

void Mouse::Update() {
  sprite->SetPosition((sf::Vector2f) sf::Mouse::GetPosition(*window));
}

void Mouse::Draw(sf::RenderTarget *rt) {
  rt->Draw(*sprite);
}
