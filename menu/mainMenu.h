#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include <vector>

#include "../screen.h"
#include "../game.h"
#include "../entity/player.h"
#include "../ui/menuItem.h"
#include "../Renderer.h"

class MainMenu : public Screen {
 public:
  MainMenu(sf::RenderWindow* , ImageManager* , Game*);
  ~MainMenu();
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
  
  //Candyeye stuff
  sf::RenderTexture* pImage;
  Player* p;
  sf::Color background;
  sf::Color nextColor;
  
  void updateColor();
  
  //TODO Remove
  Renderer* r;
};

#endif /* _MAINMENU_H_ */
