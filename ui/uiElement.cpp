#include "uiElement.h"
#include "../config.h"

UiElement::UiElement()
{
  _position = sf::Vector2f(0, 0);
}

UiElement::UiElement(ImageManager *imageManager)
{
  _position = sf::Vector2f(0, 0);
}

UiElement::~UiElement()
{
}

void UiElement::Draw(sf::RenderTarget *rt)
{
}

void UiElement::SetPosition(sf::Vector2f pos)
{
  _position = pos;
}

sf::Vector2f UiElement::GetPosition()
{
  return _position;
}
