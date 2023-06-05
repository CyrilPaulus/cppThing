#include "layerDisplay.h"
#include "../config.h"

LayerDisplay::LayerDisplay(ImageManager *imageManager, int count) : UiElement(imageManager)
{
  _layer = new sf::Sprite();
  _layer->setTexture(*(imageManager)->get("layer"));
  _layer_count = count;
  _current_layer = 0;
}

void LayerDisplay::SetLayer(int layer)
{
  this->_current_layer = layer;
}

void LayerDisplay::Draw(sf::RenderTarget *rt)
{
  _layer->setPosition(_position);
  for (int i = 0; i < _layer_count; i++)
  {
    if (i == _current_layer)
    {
      _layer->setPosition(_layer->getPosition() - sf::Vector2f(0, 10));
      rt->draw(*_layer);
      _layer->setPosition(_layer->getPosition() + sf::Vector2f(0, 10));
    }
    else
      rt->draw(*_layer);

    _layer->setPosition(_layer->getPosition() + sf::Vector2f(5, 5));
  }
}

LayerDisplay::~LayerDisplay()
{
  delete _layer;
}

sf::Vector2f LayerDisplay::GetSize()
{
  float width = _layer_count * 5 + _layer->getGlobalBounds().width;
  float height = 10 + _layer_count * 5 + _layer->getGlobalBounds().height;
  return sf::Vector2f(width, height);
}

sf::FloatRect LayerDisplay::GetBbox()
{
  return sf::FloatRect(_position.x, _position.y, GetSize().x, GetSize().y);
}
