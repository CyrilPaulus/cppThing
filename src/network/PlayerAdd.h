#pragma once

#include "packet.h"
#include <SFML/Graphics.hpp>

class PlayerAdd : public Packet{
 public:
  PlayerAdd();
  
  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setPseudo(std::string pseudo);
  void setId(int id);
  void setColor(sf::Color color);
  sf::Color getColor();
  int getId();
  std::string getPseudo();

 private:
  sf::Uint8 id;
  sf::Uint8 r,g,b;
  std::string pseudo;
};

