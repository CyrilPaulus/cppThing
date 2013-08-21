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
  Client* getClient();
  Server* getServer();
  void run(int type);
  void setIp(std::string ip);
  void setPort(int port);
  void setPseudo(std::string pseudo);

  int getType();
 private:
  sf::Thread* serverThread;
  Server* s;
  Client* c;

  bool isClient;
  bool isServer;
  bool serverRunning;
  sf::RenderWindow *window;
  ImageManager* imageManager;
  ImageManager* serverImgManager;
  std::string ip;
  std::string pseudo;
  int port;
  int _type;
  void runClient();
  void runServer();
  void startServer();
  void stopServer();
  void runLocal();
};

#endif /* _GAME_H_ */
