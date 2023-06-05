#include "packet.h"

sf::Packet Packet::encode()
{
  sf::Packet rslt;
  rslt << _type;
  return rslt;
}
