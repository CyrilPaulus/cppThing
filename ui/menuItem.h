#ifndef _MENUITEM_H_
#define _MENUITEM_H_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class MenuItem
{
public:
  MenuItem(sf::String item, sf::Vector2f position, int (*action)(void));
  int DoAction();
  virtual void Draw(sf::RenderTarget *rt);
  sf::FloatRect GetBbox();
  virtual void CenterX(int width);
  void SetActive(bool active);
  virtual bool HandleEvent(sf::Event e);

protected:
  sf::Font textFont;
  sf::Vector2f position;
  sf::Text itemText;
  int (*action)(void);
  bool active;
};

#endif /* _MENUITEM_H_ */
