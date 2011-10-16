#ifndef _MAINMENU_H_
#define _MAINMENU_H_


#include "../screen.h"
#include "../game.h"
#include "../entity/player.h"
#include "../ui/menuItem.h"

class MainMenu : public Screen {
 public:
  MainMenu(sf::RenderWindow* , ImageManager* , Game*);
  ~MainMenu();
  int Run();
  void Resize(int, int);
 private:
  Mouse* mouse;
  Game* game;
  bool running;
  int selectedItem;
  int itemCount;
  MenuItem** items;
  int HandleEvent(sf::Event e);
  int OnKeyPressed(sf::Event e);
  void OnMouseMoved(sf::Event e);
  int OnMouseButtonReleased(sf::Event e);
  
  //Candyeye stuff
  sf::RenderTexture* pImage;
  Player* p;
};

#endif /* _MAINMENU_H_ */
