#include "../config.h"
#include "usermessage.h"

UserMessage::UserMessage(unsigned int frametime, Input input, sf::Vector2f lookdir){
  this->type = Packet::UserMessage;
  this->input = input;
  this->frametime = frametime;
  this->lookdir = lookdir;
}

void UserMessage::Encode(ZCom_BitStream *message){
  message->addInt(Packet::UserMessage, 8);
  message->addBool(input.Up);
  message->addBool(input.Down);
  message->addBool(input.Left);
  message->addBool(input.Right);
  message->addInt(frametime, 32);
  message->addFloat(lookdir.x, 23);
  message->addFloat(lookdir.y, 23);
}

UserMessage* UserMessage::Decode(ZCom_BitStream &message){
  if((int) message.getInt(8) != Packet::UserMessage)
   return NULL;
  Input input;
  input.Up = message.getBool();
  input.Down = message.getBool();
  input.Left = message.getBool();
  input.Right = message.getBool();
  unsigned int frametime = message.getInt(32);
  float x = message.getFloat(23);
  float y = message.getFloat(23);
  return new UserMessage(frametime, input, sf::Vector2f(x, y));
}

unsigned int UserMessage::GetFrametime(){
  return frametime;
}

Input UserMessage::GetInput(){
  return input;
}

sf::Vector2f UserMessage::GetLookDir(){
  return lookdir;
}
