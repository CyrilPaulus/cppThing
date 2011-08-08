#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <SFML/Graphics.hpp>
#include "ticker.h"
#include "imageManager.h"
#include "mouse.h"

#include "entity/world.h"
#include "entity/player.h"

class Client {
 public:
  Client(sf::RenderWindow *window, ImageManager *imageManager);
  ~Client();
  int Run();
 private:

  void Update(float);
  void Draw();

  sf::RenderWindow *window;
  sf::RenderTexture *worldDisplay;
  Mouse *mouse;
  ImageManager *imageManager;
  Ticker *ticker;
  bool running;

  void HandleEvent(sf::Event event);
  void OnClose();
  void OnMouseButtonPressed(sf::Event event);
  void OnMouseButtonReleased(sf::Event event);
  void OnMouseWheelMoved(sf::Event event);
  void UpdateView();
  float zoom;
  int cubeType;
  Cube* displayCube;

  World *world;

  bool addCube;
  bool removeCube;
  Player *player;
};

#endif /* _CLIENT_H_ */
