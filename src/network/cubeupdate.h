#ifndef _CUBEUPDATE_H_
#define _CUBEUPDATE_H_

#include "packet.h"
#include <SFML/Graphics.hpp>

class CubeUpdate : public Packet{
 public:
  CubeUpdate(int, sf::Vector2f, bool);
  void Encode(ZCom_BitStream *);
  static CubeUpdate *Decode(ZCom_BitStream &);

  sf::Vector2f GetPosition();
  bool GetAdded();
  int GetCubeType();
 private:
  int cubeType;
  sf::Vector2f position;
  bool added;
};

#endif /* _CUBEUPDATE_H_ */