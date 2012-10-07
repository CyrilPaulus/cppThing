#ifndef _CUBE_H_
#define _CUBE_H_

#include "entity.h"
class Cube : public Entity{
 public:
  static const int WIDTH = 32;
  static const int HEIGHT = 32;
  static const int BLOCKTYPECOUNT = 48;

  Cube(int);
  void setType(int);
  int getType();
 private:
  int type;
};

#endif /* _CUBE_H_ */
