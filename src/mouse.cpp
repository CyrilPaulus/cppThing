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
  return sprite->getPosition();
}

sf::Vector2f Mouse::GetWorldPosition() {
  return world->convertCoords(sf::Vector2i(sprite->getPosition().x, sprite->getPosition().y));
}

void Mouse::Update() {
  sprite->setPosition((sf::Vector2f) sf::Mouse::getPosition(*window));
}

void Mouse::Draw(sf::RenderTarget *rt) {
  rt->draw(*sprite);
}
