#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SFML/Graphics.hpp>
#include "../imageManager.h"

class Entity {
 public:
  Entity(ImageManager *);
  ~Entity();

  void Draw(sf::RenderTarget *);
  void Update(float);
  sf::FloatRect GetBbox();
  sf::Vector2f GetPosition();
  void SetPosition(sf::Vector2f);
  sf::Vector2f GetCenter();
  

  bool CanRemove();

 protected:

  bool remove;
  
  sf::Sprite *sprite;
  sf::Vector2f position;
  sf::Vector2f offset;
  sf::Vector2f bbox;

};

#endif /* _ENTITY_H_ */
