#pragma once

#include "packet.h"


class PlayerAdd : public Packet{
 public:
  PlayerAdd();
  
  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setPseudo(std::string pseudo);
  void setId(int id);
  void setColor(sf::Vector3i color);
  sf::Vector3i getColor();
  int getId();
  std::string getPseudo();

 private:
  sf::Uint8 id;
  sf::Uint8 r,g,b;
  std::string pseudo;
};

