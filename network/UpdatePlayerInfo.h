#pragma once

#include "packet.h"

class UpdatePlayerInfo : public Packet {
 public:
  UpdatePlayerInfo();
  
  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setPseudo(std::string pseudo);
  void setId(int id);
  void setColor(sf::Vector3i color);
  sf::Vector3i getColor();
  int getId();
  std::string getPseudo();

 private:
  sf::Uint8 _id;
  sf::Uint8 _r,_g,_b;
  std::string _pseudo;
};

