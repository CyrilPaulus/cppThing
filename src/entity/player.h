#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include "entity.h"
#include "world.h"
#include "../input/input.h"

class Player : public Entity {
 public:
  Player(ImageManager*, World*);
  ~Player();
  void Draw(sf::RenderTarget *);
  void SetEyesPosition(sf::Vector2f);
  void Update(float, Input);

  static void RegisterClass(ZCom_Control * control, bool server);

 private:
  World* world;
  
  bool noclip;
  bool isFlying;
  sf::Sprite *pupil;
  sf::Sprite *colorMask;

  sf::Vector2f lpOrigin;
  sf::Vector2f lpPosition;
  sf::Vector2f rpOrigin;
  sf::Vector2f rpPosition;

  float maxWalkSpeed;
  float maxFallSpeed;
  float jumpForce;

  sf::Vector2f speed;
  sf::Vector2f acceleration;
};

#endif /* _PLAYER_H_ */
