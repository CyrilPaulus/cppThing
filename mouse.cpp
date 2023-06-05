#include "config.h"
#include "mouse.h"

Mouse::Mouse(sf::RenderWindow *window, sf::RenderTarget *world, ImageManager *image_manager)
{
  _window = window;
  _world = world;
  _sprite = new sf::Sprite(*(image_manager->get("mouse")));
}

Mouse::~Mouse()
{
  delete _sprite;
}

sf::Vector2f Mouse::getPosition()
{
  return _sprite->getPosition();
}

sf::Vector2f Mouse::getWorldPosition()
{
  return _world->mapPixelToCoords(sf::Vector2i(_sprite->getPosition().x, _sprite->getPosition().y));
}

void Mouse::update()
{
  _sprite->setPosition((sf::Vector2f)sf::Mouse::getPosition(*_window));
}

void Mouse::draw(sf::RenderTarget *rt)
{
  rt->draw(*_sprite);
}
