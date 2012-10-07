#pragma once

#include "packet.h"

class PlayerDelete : public Packet{
 public:
  PlayerDelete();
  
  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setId(int id);
  int getId();

 private:
  sf::Uint8 id;
};

