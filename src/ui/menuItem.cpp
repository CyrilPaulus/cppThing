#include "menuItem.h"
#include "../config.h"

MenuItem::MenuItem(sf::String item, sf::Vector2f position, int (*action)(void)) {
  this->position = position;
  this->action = action;
  //TODO optimize this
  this->textFont.LoadFromFile("res/arial.ttf");
  this->itemText = sf::Text(item, textFont);
  this->itemText.SetPosition(position);
}

int MenuItem::DoAction() {
  return this->action();
}

void MenuItem::Draw(sf::RenderTarget *rt, bool selected) {
  if(selected)
    itemText.SetColor(sf::Color(255, 201, 14));
  else
    itemText.SetColor(sf::Color(255, 255, 255));
  rt->Draw(itemText);
}

sf::FloatRect MenuItem::GetBbox() {
  return itemText.GetRect();
}

void MenuItem::CenterX(int width) {
  position = sf::Vector2f((width - itemText.GetRect().Width)/2, position.y);
  itemText.SetPosition(position);
}

