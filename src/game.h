#ifndef _GAME_H_
#define _GAME_H_

#include <SFML/Graphics.hpp>
#include "imageManager.h"

#include "client.h"
#include "server.h"

class Game {
 public:
  static const int LOCAL = 0;
  static const int CLIENT = 1;
  static const int SERVER = 2;
  Game();
  ~Game();
  void Run(int type);

 private:
  bool isClient;
  bool isServer;
  sf::RenderWindow *window;
  ImageManager* imageManager;
};

#endif /* _GAME_H_ */
