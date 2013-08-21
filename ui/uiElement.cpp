#include "uiElement.h"
#include "../config.h"

UiElement::UiElement() {
  position = sf::Vector2f(0, 0);
}

UiElement::UiElement(ImageManager *imageManager) {
  position = sf::Vector2f(0, 0);
}

UiElement::~UiElement() {
}

void UiElement::Draw(sf::RenderTarget *rt) {
}

void UiElement::SetPosition(sf::Vector2f pos) {
  this->position = pos;  
}

sf::Vector2f UiElement::GetPosition() {
  return position;
} 
