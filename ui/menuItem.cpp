#include "menuItem.h"
#include "../screen.h"
#include "../config.h"

MenuItem::MenuItem(sf::String item, sf::Vector2f position, int (*action)(void))
{
  this->position = position;
  this->action = action;
  // TODO optimize this
  this->textFont.loadFromFile("res/arial.ttf");
  this->itemText = sf::Text(item, textFont);
  this->itemText.setPosition(position);
  this->active = false;
}

int MenuItem::DoAction()
{
  if (action != NULL)
    return this->action();
  return Screen::NONE;
}

void MenuItem::Draw(sf::RenderTarget *rt)
{
  if (active)
    itemText.setColor(sf::Color(255, 201, 14));
  else
    itemText.setColor(sf::Color(255, 255, 255));
  rt->draw(itemText);
}

sf::FloatRect MenuItem::GetBbox()
{
  return itemText.getGlobalBounds();
}

void MenuItem::CenterX(int width)
{
  position = sf::Vector2f((width - itemText.getGlobalBounds().width) / 2, position.y);
  itemText.setPosition(position);
}

void MenuItem::SetActive(bool active)
{
  this->active = active;
}

bool MenuItem::HandleEvent(sf::Event e)
{
  return false;
}
