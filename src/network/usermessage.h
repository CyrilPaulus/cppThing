#ifndef _USERMESSAGE_H_
#define _USERMESSAGE_H_
#include "packet.h"
#include "../input/input.h"

class UserMessage : public Packet{
 public:
  UserMessage(float, Input, sf::Vector2f lookdir);
  void Encode(ZCom_BitStream *);
  static UserMessage *Decode(ZCom_BitStream &);

  float GetFrametime();
  Input GetInput();
  sf::Vector2f GetLookDir();
 private:
  float frametime;
  Input input;
  sf::Vector2f lookdir;
};


#endif /* _USERMESSAGE_H_ */
