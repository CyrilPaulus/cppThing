#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include "entity.h"
#include "world.h"
#include "../input/input.h"

class World;

class Player : public Entity
{
public:
  Player(World *);
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
  World *_world;

  bool _noclip;
  bool _is_flying;

  sf::Vector2f _lp_origin;
  sf::Vector2f _lp_position;
  sf::Vector2f _rp_origin;
  sf::Vector2f _rp_position;

  float _max_walk_speed;
  float _max_fall_speed;
  float _jump_force;
  sf::Time _acc;

  sf::Vector2f _velocity;
  sf::Vector2f _acceleration;
  sf::Vector3i _color;

  int _id;
  std::string _pseudo;
};

#endif /* _PLAYER_H_ */
