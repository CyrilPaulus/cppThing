#include "../config.h"
#include "entity.h"

Entity::Entity()
{
  _position = sf::Vector2f(0, 0);
  _bbox = sf::Vector2f(0, 0);
  _remove = false;
}

Entity::~Entity()
{
}

void Entity::update(float frametime)
{
}

sf::FloatRect Entity::getBbox()
{
  return sf::FloatRect(_position.x, _position.y, _bbox.x, _bbox.y);
}

sf::Vector2f Entity::getPosition()
{
  return _position;
}

sf::Vector2f Entity::getCenter()
{
  sf::Vector2f hbbox = sf::Vector2f(_bbox.x / 2, _bbox.y / 2);
  return getPosition() + hbbox;
}

void Entity::setPosition(sf::Vector2f position)
{
  _position = position;
}

bool Entity::canRemove()
{
  return _remove;
}
