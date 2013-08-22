#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <SFML/Graphics.hpp>
#include "imageManager.h"

class Mouse{
 public:
  Mouse(sf::RenderWindow*, sf::RenderTarget*, ImageManager*);
  ~Mouse();
  void draw(sf::RenderTarget *);
  void update();
  sf::Vector2f getPosition();
  sf::Vector2f getWorldPosition();

 private:
  sf::RenderWindow* _window;
  sf::RenderTarget* _world;
  sf::Sprite* _sprite;
  
  
};

#endif /* _MOUSE_H_ */
