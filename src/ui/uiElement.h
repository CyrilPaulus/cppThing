#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_

#include <SFML/Graphics.hpp>
#include "../imageManager.h"

class UiElement{
 public:
  UiElement(ImageManager*);
  ~UiElement();
  
  void Draw(sf::RenderTarget *);
  void SetPosition(sf::Vector2f);
  sf::Vector2f GetPosition();

 protected:
  sf::Vector2f position;
  
};

#endif /* _UIELEMENT_H_ */
