#ifndef _PACKET_H_
#define _PACKET_H_

#include <zoidcom.h>

class Packet{
 public:
  static const int Default = 0;
  static const int CubeUpdate = 1;
 protected:
  int type;
};

#endif /* _PACKET_H_ */
