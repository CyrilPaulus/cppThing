#ifndef _PACKET_H_
#define _PACKET_H_

#include <SFML/Network.hpp>

class Packet
{
public:
  static const sf::Uint8 Default = 0;
  static const sf::Uint8 UserMessage = 1;
  static const sf::Uint8 CubeUpdate = 2;
  static const sf::Uint8 ClientConnect = 3;
  static const sf::Uint8 AddPlayer = 4;
  static const sf::Uint8 DeletePlayer = 5;
  static const sf::Uint8 UpdatePlayer = 6;
  static const sf::Uint8 TextMessage = 7;
  static const sf::Uint8 UpdatePlayerInfo = 8;

  virtual sf::Packet encode();

protected:
  sf::Uint8 _type;
};

#endif /* _PACKET_H_ */
