#include "menuTextInput.h"

MenuTextInput::MenuTextInput(sf::String item, sf::Vector2f position, int (*action)(void)) : MenuItem(item, position, action) {
  _label = item;
  _displayUnderscore = false;
}

void MenuTextInput::Draw(sf::RenderTarget *rt) {

  sf::Time cur = _clock.getElapsedTime();
  if(cur.asSeconds() > 0.5) {
    _displayUnderscore = !_displayUnderscore;
    _clock.restart();
  }

  if(active && _displayUnderscore)
    itemText.setString(_label + ": " + _input + "_");
  else 
    itemText.setString(_label + ": " + _input);

  if(active)
    itemText.setColor(sf::Color(255, 201, 14));
  else
    itemText.setColor(sf::Color(255, 255, 255));

  
  rt->draw(itemText);
}

bool MenuTextInput::HandleEvent(sf::Event event) {
  if (active && event.type == sf::Event::KeyPressed) {
    if(event.key.code == sf::Keyboard::BackSpace) {
      if(_input.getSize() > 0) 
	_input.erase(_input.getSize() - 1);
      return true;
    }
  } else if(active && event.type == sf::Event::TextEntered) {
    char in = static_cast<char>(event.text.unicode);
    
    if(in != '\b' && in != '\r')
      _input.insert(_input.getSize(), event.text.unicode);
    return true;
  }

  return false;
}

void MenuTextInput::CenterX(int width) {

  //Ignore underscore
  itemText.setString(_label + ": " + _input);
  position = sf::Vector2f((width - itemText.getGlobalBounds().width)/2, position.y);
  itemText.setPosition(position);
}

std::string MenuTextInput::GetText() {
  return _input.toAnsiString();
}

void MenuTextInput::SetText(std::string text) {
  _input = sf::String(text);
}
