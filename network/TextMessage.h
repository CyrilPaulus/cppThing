#pragma once

#include "packet.h"

class TextMessage : public Packet
{
public:
  TextMessage();

  virtual sf::Packet encode();
  void decode(sf::Packet p);

  void setMessage(std::string message);
  std::string getMessage();

private:
  std::string _message;
};
