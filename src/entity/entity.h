#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SFML/Graphics.hpp>
#include "../imageManager.h"

class Entity {
 public:
  Entity();
  virtual ~Entity();

  virtual void update(float);
  sf::FloatRect getBbox();
  sf::Vector2f getPosition();
  void setPosition(sf::Vector2f);
  sf::Vector2f getCenter();

  bool canRemove();

 protected:
  bool remove;
  sf::Vector2f position;
  sf::Vector2f bbox;

};

#endif /* _ENTITY_H_ */
