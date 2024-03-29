#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_

#include <SFML/Graphics.hpp>
#include "../imageManager.h"

class UiElement
{
public:
  UiElement(ImageManager *);
  UiElement();
  ~UiElement();

  void Draw(sf::RenderTarget *);
  void SetPosition(sf::Vector2f);
  sf::Vector2f GetPosition();
  sf::FloatRect GetBbox();
  sf::Vector2f GetSize();

protected:
  sf::Vector2f _position;
};

#endif /* _UIELEMENT_H_ */
