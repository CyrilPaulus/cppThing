#ifndef _PACKET_H_
#define _PACKET_H_


class Packet{
 public:
  static const int Default = 0;
  static const int CubeUpdate = 1;
  static const int UserMessage = 2;
 protected:
  int type;
};

#endif /* _PACKET_H_ */
