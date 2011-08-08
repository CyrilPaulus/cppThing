#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include "entity.h"
#include "world.h"

class Player : public Entity {
 public:
  Player(World*, ImageManager*);
  ~Player();
  void Draw(sf::RenderTarget *);
  void SetEyesPosition(sf::Vector2f);

 private:
  World* world;
  sf::Sprite *pupil;
  sf::Sprite *colorMask;

  sf::Vector2f lpOrigin;
  sf::Vector2f lpPosition;
  sf::Vector2f rpOrigin;
  sf::Vector2f rpPosition;
};

#endif /* _PLAYER_H_ */
