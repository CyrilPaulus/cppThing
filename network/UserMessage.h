#ifndef _USERMESSAGE_H_
#define _USERMESSAGE_H_
#include "packet.h"
#include "../input/input.h"

class UserMessage : public Packet
{
public:
  UserMessage(unsigned int, Input, sf::Vector2f lookdir);

  unsigned int getFrametime();
  Input getInput();
  sf::Vector2f getLookDir();

private:
  float _frametime;
  Input _input;
  sf::Vector2f _look_dir;
};

#endif /* _USERMESSAGE_H_ */
