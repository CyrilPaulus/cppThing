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

#include "network/packet.h"
class Client : public Screen{
 public:
  Client(sf::RenderWindow *window, ImageManager *imageManager);
  ~Client();
  int run();
  void connect();
  void disconnect();
 
  void setPort(int);
  void setIp(std::string);

 private:
  int layer;
  void update(sf::Time frametime);
  void draw();

  std::string pseudo;  
  sf::RenderTexture *worldDisplay;
  Mouse *mouse;  
  Ticker *ticker;
  bool running;

  void handleEvent(sf::Event event);
  void handlePacket(sf::Packet p);
  void onClose();
  void onMouseButtonPressed(sf::Event event);
  void onMouseButtonReleased(sf::Event event);
  void onMouseWheelMoved(sf::Event event);
  void onResized(sf::Event event);
  void onKeyPressed(sf::Event event);

  void sendReliable(Packet* p);
  void send(Packet* p);

  void updateView();
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
  int id;
};

#endif /* _CLIENT_H_ */
