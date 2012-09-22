#pragma once

#include "packet.h"
#include <SFML/Graphics.hpp>

class DeletePlayer : public Packet{
 public:
  DeletePlayer();
  
  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setId(int id);
  int getId();

 private:
  sf::Uint8 id;
};

