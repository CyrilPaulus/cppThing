#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "../screen.h"
#include "../game.h"
#include "../ui/menuItem.h"

class MainMenu : public Screen {
 public:
  MainMenu(sf::RenderWindow* , ImageManager* , Game*);
  ~MainMenu();
  int Run();

 private:
  Game* game;
  bool running;
  int selectedItem;
  int itemCount;
  MenuItem** items;

};

#endif /* _MAINMENU_H_ */
