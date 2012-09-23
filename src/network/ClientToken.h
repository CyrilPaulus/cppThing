#pragma once

#include "packet.h"
#include <SFML/Graphics.hpp>

class ClientToken : public Packet{
 public:
  ClientToken();
  
  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setId(int id);
  int getId();

 private:
  sf::Uint8 id;
};

