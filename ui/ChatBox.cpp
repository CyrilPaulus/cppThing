#include "ChatBox.h"

ChatBox::ChatBox() : UiElement() {
  _textFont.loadFromFile("res/arial.ttf");
  _nbr_lines = 15;
  _active = false;
  _fade_time = sf::seconds(5);
  _first = false;
}

void ChatBox::Draw(sf::RenderTarget* rt) {
  
  if(!_active && _clock.getElapsedTime() > _fade_time) {
    return;
  }
  
  for(int i = _msg.size() - 1; i >= 0 && i >(int) _msg.size() -1 - _nbr_lines; i--) { 
    sf::Text text = sf::Text(_msg[i], _textFont, 15);
    text.setPosition(position -sf::Vector2f(0,((int)_msg.size() + 1 - i) * _textFont.getLineSpacing(15)));
    rt->draw(text);
  }

  if(_active) {
    sf::Text text = sf::Text(_typing_msg + "_", _textFont, 15);
    text.setPosition(position - sf::Vector2f(0, _textFont.getLineSpacing(15)));
    rt->draw(text);
  }

}

void ChatBox::handleEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {
    
    if(!_active && event.key.code == sf::Keyboard::Y) {
      _active = true;
      _first = true;
      _typing_msg = "";
      
    } else if(_active && event.key.code == sf::Keyboard::Return) {
      _active = false;
      if(_typing_msg.getSize() > 0)
	_pending_msg.push_back(_typing_msg);
      _typing_msg = "";
    } else if(_active && event.key.code == sf::Keyboard::BackSpace) {
      if(_typing_msg.getSize() > 0)
	_typing_msg.erase(_typing_msg.getSize() - 1);
    }
  } else if(_active && event.type == sf::Event::TextEntered) {
    //Remove y
    if(_first) 
      _first = false;
    else if(static_cast<char>(event.text.unicode) != '\b')
      _typing_msg.insert(_typing_msg.getSize(), event.text.unicode);
  }
}


int ChatBox::getPendingCount() {
  return _pending_msg.size();
}

std::string ChatBox::popPendingMsg() {
  std::string front = _pending_msg.front();
  _pending_msg.pop_front();
  return front;
}

void ChatBox::addMessage(std::string msg) {
  _clock.restart();
  _msg.push_back(msg);
}

bool ChatBox::isActive() {
  return _active;
}
