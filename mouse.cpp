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

sf::Vector2f Mouse::getPosition() {
  return sprite->getPosition();
}

sf::Vector2f Mouse::getWorldPosition() {
  return world->mapPixelToCoords(sf::Vector2i(sprite->getPosition().x, sprite->getPosition().y));
}

void Mouse::update() {
  sprite->setPosition((sf::Vector2f) sf::Mouse::getPosition(*window));
}

void Mouse::draw(sf::RenderTarget *rt) {
  rt->draw(*sprite);
}
