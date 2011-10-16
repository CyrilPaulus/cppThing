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
  void Resize(int, int);
 private:
  Game* game;
  bool running;
  int selectedItem;
  int itemCount;
  MenuItem** items;
  int HandleEvent(sf::Event e);
  int OnKeyPressed(sf::Event e);
};

#endif /* _MAINMENU_H_ */
