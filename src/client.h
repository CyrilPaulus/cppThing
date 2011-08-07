#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <SFML/Graphics.hpp>
#include "ticker.h"
#include "imageManager.h"
#include "mouse.h"

class Client {
 public:
  Client(sf::RenderWindow *window, ImageManager *imageManager);
  ~Client();
  int Run();
 private:

  void Update(float);
  void Draw();

  sf::RenderWindow *window;
  sf::RenderImage *ui;
  Mouse *mouse;
  ImageManager *imageManager;
  Ticker *ticker;
  bool running;

  void HandleEvent(sf::Event event);
  void OnClose();
};

#endif /* _CLIENT_H_ */
