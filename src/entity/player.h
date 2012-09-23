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
  virtual ~Player();
  virtual void draw(sf::RenderTarget *);
  void setEyesPosition(sf::Vector2f);
  void update(sf::Time frametime, Input);
  void setColor(sf::Color);
  sf::Color getColor();
  void setId(int);
  int getId();
  void setPseudo(std::string pseudo);
  std::string getPseudo();
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
  sf::Time acc;

  sf::Vector2f velocity;
  sf::Vector2f acceleration;
  
  int id;
  std::string pseudo;
};

#endif /* _PLAYER_H_ */
