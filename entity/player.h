#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include "entity.h"
#include "world.h"
#include "../input/input.h"

class World;

class Player : public Entity {
 public:
  Player(World*);
  virtual ~Player();
 
  void setEyesPosition(sf::Vector2f);
  void update(sf::Time frametime, Input);
  void setColor(sf::Vector3i);
  sf::Vector3i getColor();
  
  void setId(int);
  int getId();
  
  void setPseudo(std::string pseudo);
  std::string getPseudo();
  
  sf::Vector2f getLeftPupilPosition();
  sf::Vector2f getRightPupilPosition();
 private:
  World* world;
  
  bool noclip;
  bool isFlying;

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
  sf::Vector3i color;
  
  int id;
  std::string pseudo;
};

#endif /* _PLAYER_H_ */
