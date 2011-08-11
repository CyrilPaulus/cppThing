#ifndef _USERMESSAGE_H_
#define _USERMESSAGE_H_
#include "packet.h"
#include "../input/input.h"

class UserMessage : public Packet{
 public:
  UserMessage(unsigned int, Input, sf::Vector2f lookdir);
  void Encode(ZCom_BitStream *);
  static UserMessage *Decode(ZCom_BitStream &);

  unsigned int GetFrametime();
  Input GetInput();
  sf::Vector2f GetLookDir();
 private:
  float frametime;
  Input input;
  sf::Vector2f lookdir;
};


#endif /* _USERMESSAGE_H_ */