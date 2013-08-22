#include "../config.h"
#include "UserMessage.h"

UserMessage::UserMessage(unsigned int frametime, Input input, sf::Vector2f look_dir){
  _type = Packet::UserMessage;
  _input = input;
  _frametime = frametime;
  _look_dir = look_dir;
}

unsigned int UserMessage::getFrametime(){
  return _frametime;
}

Input UserMessage::getInput(){
  return _input;
}

sf::Vector2f UserMessage::getLookDir(){
  return _look_dir;
}
