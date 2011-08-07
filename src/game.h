#ifndef _GAME_H_
#define _GAME_H_

#include <SFML/Graphics.hpp>
#include "imageManager.h"
#include "client.h"

class Game {
 public:
  Game();
  ~Game();
  void Run();

 private:
  bool isClient;
  bool isServer;
  sf::RenderWindow *window;
  ImageManager* imageManager;

};

#endif /* _GAME_H_ */
