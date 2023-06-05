#ifndef _OPTIONMENU_H_
#define _OPTIONMENU_H_

#include <vector>

#include "../screen.h"
#include "../game.h"
#include "../ui/menuItem.h"
#include "../ui/menuTextInput.h"
#include "../Renderer.h"

class OptionMenu : public Screen
{
public:
  OptionMenu(sf::RenderWindow *, ImageManager *, Game *);
  ~OptionMenu();
  int run();
  void resize(int, int);

private:
  Mouse *mouse;
  Game *game;
  bool running;
  unsigned int selectedItem;
  std::vector<MenuItem *> items;
  int handleEvent(sf::Event e);
  int onKeyPressed(sf::Event e);
  void onMouseMoved(sf::Event e);
  int onMouseButtonReleased(sf::Event e);

  MenuTextInput *_name;
  MenuTextInput *_color;
  // TODO Remove
  Renderer *r;
};

#endif /* _CONNECTMENU_H_ */
