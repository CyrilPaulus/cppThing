#include "packet.h"

sf::Packet Packet::encode() {
  sf::Packet rslt;
  rslt << type;
  return rslt;
}
