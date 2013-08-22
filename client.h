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
#include "ui/ChatBox.h"

#include "network/packet.h"
#include "Renderer.h"

class Client : public Screen{
 public:
  Client(sf::RenderWindow *window, ImageManager *imageManager);
  ~Client();
  int run();
  void connect();
  void disconnect();
  
  void setPort(int);
  void setIp(std::string);
  void setPseudo(std::string pseudo);
  
 private:
  void update(sf::Time frametime);
  void draw();

  int _layer;
  std::string _pseudo;  
  sf::RenderTexture *_world_display;
  Mouse *_mouse;  
  Ticker *_ticker;
  bool _running;

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

  float _zoom;
  int _cube_type;
  CubeDisplay* _display_cube;
  ChatBox* _chat_box;
  Renderer* _renderer;
  
  World* _world;  

  bool _add_cube;
  bool _remove_cube;
  bool _main_menu;
  Player* _player;
  LayerDisplay* _layer_display;
  int _port;
  std::string _ip;
  ENetHost* _client;
  ENetPeer* _server;
  bool _connected;
  int _id;

  bool _has_focus;
};

#endif /* _CLIENT_H_ */
