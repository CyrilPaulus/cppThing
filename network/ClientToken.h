#pragma once

#include "packet.h"

class ClientToken : public Packet
{
public:
  ClientToken();

  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setId(int id);
  int getId();

private:
  sf::Uint8 _id;
};
