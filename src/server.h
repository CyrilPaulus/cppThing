#ifndef _SERVER_H_
#define _SERVER_H_

#include <SFML/Graphics.hpp>

#include <list>
#include "ticker.h"
#include "imageManager.h"
#include "entity/world.h"
#include "network/NetworkClient.h"


class Server {
 public:
  Server(ImageManager *);
  ~Server();
  void Run();
  void Stop();
  void Update(sf::Time frametime);
  void Init();
  void SetPort(int port);
 
 private:
  void addClient(unsigned int ip, unsigned int port);
  void removeClient(unsigned int ip, unsigned int port);
  bool running;
  ImageManager *imageManager;
  Ticker *ticker;
  World *world;
  int port;
  int maxClient;
  int lastClientID;
  std::list<NetworkClient> clients;
};

#endif /* _SERVER_H_ */
