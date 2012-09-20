#include "menuItem.h"
#include "../config.h"

MenuItem::MenuItem(sf::String item, sf::Vector2f position, int (*action)(void)) {
  this->position = position;
  this->action = action;
  //TODO optimize this
  this->textFont.loadFromFile("res/arial.ttf");
  this->itemText = sf::Text(item, textFont);
  this->itemText.setPosition(position);
}

int MenuItem::DoAction() {
  return this->action();
}

void MenuItem::Draw(sf::RenderTarget *rt, bool selected) {
  if(selected)
    itemText.setColor(sf::Color(255, 201, 14));
  else
    itemText.setColor(sf::Color(255, 255, 255));
  rt->draw(itemText);
}

sf::FloatRect MenuItem::GetBbox() {
  return itemText.getGlobalBounds();
}

void MenuItem::CenterX(int width) {
  position = sf::Vector2f((width - itemText.getGlobalBounds().width)/2, position.y);
  itemText.setPosition(position);
}

