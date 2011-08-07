#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <SFML/Graphics.hpp>
#include "imageManager.h"

class Mouse{
 public:
  Mouse(sf::RenderWindow*, ImageManager*);
  ~Mouse();
  void Draw(sf::RenderTarget *);
  void Update();
  sf::Vector2f getPosition();
  sf::Vector2f getWorldPosition();

 private:
  sf::RenderWindow *window;
  ImageManager *imageManager;
  sf::Sprite *sprite;
  
};

#endif /* _MOUSE_H_ */
