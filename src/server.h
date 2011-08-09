#ifndef _SERVER_H_
#define _SERVER_H_

#include <zoidcom.h>
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
  void Update(float);
 private:
  bool running;
  ImageManager *imageManager;
  Ticker *ticker;
  World *world;
  
};

#endif /* _SERVER_H_ */
