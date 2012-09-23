#pragma once

#include "packet.h"
#include <SFML/Graphics.hpp>

class PlayerUpdate : public Packet{
 public:
  PlayerUpdate();
  
  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setPosition(sf::Vector2f position);
  void setEyePosition(sf::Vector2f position);
  void setId(int id);
  int getId();

  sf::Vector2f getPosition();
  sf::Vector2f getEyePosition();
 private:
  sf::Uint8 id;
  sf::Vector2f position;
  sf::Vector2f eyePosition;
};

