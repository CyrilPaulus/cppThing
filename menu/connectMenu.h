#ifndef _CONNECTMENU_H_
#define _CONNECTMENU_H_

#include <vector>

#include "../screen.h"
#include "../game.h"
#include "../ui/menuItem.h"
#include "../Renderer.h"

class ConnectMenu : public Screen {
 public:
  ConnectMenu(sf::RenderWindow* , ImageManager* , Game*);
  ~ConnectMenu();
  int run();
  void resize(int, int);

 private:
  Mouse* mouse;
  Game* game;
  bool running;
  unsigned int selectedItem;
  std::vector<MenuItem*> items;
  int handleEvent(sf::Event e);
  int onKeyPressed(sf::Event e);
  void onMouseMoved(sf::Event e);
  int onMouseButtonReleased(sf::Event e);
  
  //TODO Remove
  Renderer* r;
};

#endif /* _CONNECTMENU_H_ */
