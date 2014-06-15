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
  int getType(); 
  
  void run(int type);
  void setIp(std::string ip);
  void setPort(int port);
  void setPseudo(std::string pseudo);
  void UpdatePlayerInfo(std::string name, sf::Vector3i color);
  std::string getPseudo();
  sf::Vector3i getColor();
  void connect(std::string ip);
  std::string getIpStr();

 private:
  sf::Thread* _server_thread;
  Server* _s;
  Client* _c;

  bool _server_running;
  sf::RenderWindow* _window;
  ImageManager* _image_manager;
  std::string _ip;
  std::string _pseudo;
  int _port;
  int _type;

  void runClient();
  void runServer();
  void startServer();
  void stopServer();
  void runLocal();

  Screen* screens[6];
};

#endif /* _GAME_H_ */
