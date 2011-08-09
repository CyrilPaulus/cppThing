#ifndef _CUBE_H_
#define _CUBE_H_

#include "entity.h"
class Cube : public Entity{
 public:
  static const int WIDTH = 32;
  static const int HEIGHT = 32;
  static const int BLOCKTYPECOUNT = 48;

  Cube(ZCom_Control * ,int,sf::Vector2f, ImageManager*, bool);
  void SetType(int);
  static void RegisterClass(ZCom_Control * control, bool server);
 private:
  int type;
  ImageManager *imageManager;
    
};

#endif /* _CUBE_H_ */
