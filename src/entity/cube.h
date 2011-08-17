#ifndef _CUBE_H_
#define _CUBE_H_

#include "entity.h"
class Cube : public Entity{
 public:
  static const int WIDTH = 32;
  static const int HEIGHT = 32;
  static const int BLOCKTYPECOUNT = 48;

  Cube(ImageManager*, int);
  void SetType(int);
  static void RegisterClass(ZCom_Control * , bool);
  void RegisterZCom(ZCom_Control *, bool, int);
  static ZCom_ClassID GetClass(bool server);
 private:
  int type;
  ImageManager *imageManager;
    
  static ZCom_ClassID netClassServerId;
  static ZCom_ClassID netClassClientId;
};

#endif /* _CUBE_H_ */
