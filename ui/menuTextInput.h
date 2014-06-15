#ifndef _MENUTEXTINPUT_H_
#define _MENUTEXTINPUT_H_

#include "menuItem.h"

class MenuTextInput : public MenuItem {
 public:
    MenuTextInput(sf::String item, sf::Vector2f position, int (*action)(void));
    bool HandleEvent(sf::Event e);
    void Draw(sf::RenderTarget *rt);
    void CenterX(int width);
    std::string GetText();
    void SetText(std::string text);

 private:
    sf::String _input;
    sf::String _label;
    bool _displayUnderscore;
    sf::Clock _clock;
};

#endif /* _MENUITEM_H_ */
