#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <SFML/Graphics.hpp>
#include <enet/enet.h>
#include "screen.h"

#include "ticker.h"
#include "imageManager.h"
#include "mouse.h"
#include "config.h"

#include "entity/world.h"
#include "entity/player.h"

#include "ui/layerDisplay.h"
#include "ui/cubeDisplay.h"

class Client : public Screen{
 public:
  Client(sf::RenderWindow *window, ImageManager *imageManager);
  ~Client();
  int Run();
  void Connect();
  void Disconnect();
 
  void SetPort(int);
  void SetIp(std::string);

 private:
  int layer;
  void Update(sf::Time frametime);
  void Draw();

  std::string pseudo;  
  sf::RenderTexture *worldDisplay;
  Mouse *mouse;  
  Ticker *ticker;
  bool running;

  void HandleEvent(sf::Event event);
  void OnClose();
  void OnMouseButtonPressed(sf::Event event);
  void OnMouseButtonReleased(sf::Event event);
  void OnMouseWheelMoved(sf::Event event);
  void OnResized(sf::Event event);
  void OnKeyPressed(sf::Event event);

  void UpdateView();
  float zoom;
  int cubeType;
  CubeDisplay* displayCube;

  World *world;

  bool addCube;
  bool removeCube;
  bool mainMenu;
  Player *player;
  LayerDisplay * layerDisplay;
  int port;
  std::string ip;
  ENetHost* client;
  ENetPeer* server;
  bool connected;
};

#endif /* _CLIENT_H_ */
