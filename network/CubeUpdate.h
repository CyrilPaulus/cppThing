#ifndef _CUBEUPDATE_H_
#define _CUBEUPDATE_H_

#include "packet.h"

class CubeUpdate : public Packet{
 public:
  CubeUpdate();
  CubeUpdate(int, sf::Vector2f, bool, int);
  //  void Encode(ZCom_BitStream *);
  //  static CubeUpdate *Decode(ZCom_BitStream &);

  virtual sf::Packet encode();
  void decode(sf::Packet p);
  sf::Vector2f GetPosition();
  bool GetAdded();
  int GetCubeType();
  int GetLayer();
 private:
  int layer;
  int cubeType;
  sf::Vector2f position;
  bool added;
};

#endif /* _CUBEUPDATE_H_ */
