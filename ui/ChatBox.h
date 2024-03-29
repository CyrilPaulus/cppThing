#ifndef _CHATBOX_H_
#define _CHATBOX_H_

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <string>
#include <vector>
#include <list>

#include "uiElement.h"
#include "../Renderer.h"

class ChatBox : public UiElement
{
public:
  ChatBox();
  void Draw(sf::RenderTarget *);
  void handleEvent(sf::Event event);
  std::string popPendingMsg();
  int getPendingCount();
  void addMessage(std::string msg);
  bool isActive();

private:
  sf::Font _textFont;
  std::vector<std::string> _msg;
  std::list<std::string> _pending_msg;
  sf::String _typing_msg;
  int _nbr_lines;
  bool _active;
  bool _first;
  sf::Clock _clock;
  sf::Clock _underClock;
  bool _displayUnderscore;
  sf::Time _fade_time;
};

#endif
