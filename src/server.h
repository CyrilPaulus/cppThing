#ifndef _SERVER_H_
#define _SERVER_H_

#include <SFML/Graphics.hpp>

#include "ticker.h"
#include "imageManager.h"
#include "entity/world.h"

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
  bool running;
  ImageManager *imageManager;
  Ticker *ticker;
  World *world;
  int port;
  
};

#endif /* _SERVER_H_ */
