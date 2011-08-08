#include "config.h"
#include "mouse.h"

Mouse::Mouse(sf::RenderWindow *window, ImageManager *imageManager) {
  this->window = window;
  this->imageManager = imageManager;
  sf::Image *image = imageManager->get("mouse");
  this->sprite = new sf::Sprite(*(image));
}

Mouse::~Mouse() {
  delete sprite;
}

sf::Vector2f Mouse::GetPosition() {
  return sprite->GetPosition();
}

sf::Vector2f Mouse::GetWorldPosition() {
  return window->ConvertCoords((uint) sprite->GetPosition().x, (uint) sprite->GetPosition().y);
}

void Mouse::Update() {
  sprite->SetPosition((sf::Vector2f) sf::Mouse::GetPosition(*window));
}

void Mouse::Draw(sf::RenderTarget *rt) {
  rt->Draw(*sprite);
}
