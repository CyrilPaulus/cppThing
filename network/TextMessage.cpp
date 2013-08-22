#include "TextMessage.h"

TextMessage::TextMessage() {
  _type = Packet::TextMessage;
}

sf::Packet TextMessage::encode() {
  sf::Packet rslt = Packet::encode();
  rslt << _message;
  return rslt;
}

void TextMessage::decode(sf::Packet p) {
  p >> _message;
}

void TextMessage::setMessage(std::string message) {
  _message = message;
}

std::string TextMessage::getMessage() {
  return _message;
}
