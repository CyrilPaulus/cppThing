#ifndef _MENUITEM_H_
#define _MENUITEM_H_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class MenuItem {
 public:
  MenuItem(sf::String item, sf::Vector2f position, int (*action)(void));
  int DoAction();
  void Draw(sf::RenderTarget *rt, bool selected);
  sf::FloatRect GetBbox();
  void CenterX(int width);
    
 private:
  sf::Font textFont;
  sf::Vector2f position;
  sf::Text itemText;
  int (*action)(void);
};

#endif /* _MENUITEM_H_ */
