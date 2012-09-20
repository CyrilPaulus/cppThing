#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include "entity.h"
#include "world.h"
#include "../input/input.h"

class World;

class Player : public Entity {
 public:
  Player(ImageManager*, World*);
  ~Player();
  void Draw(sf::RenderTarget *);
  void SetEyesPosition(sf::Vector2f);
  void Update(sf::Time frametime, Input);
  void InputUpdate(Input);
  void PhysicUpdate(sf::Time frametime);
  void SetColor(sf::Color);
  void SetID(int);
  int GetID();
 private:
  void DoOwner(Input input, unsigned int frametime);
  void DoProxy();
  void DoAuth();
  World* world;
  
  bool moveX;
  bool moveY;

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
  
  int id;
};

#endif /* _PLAYER_H_ */
