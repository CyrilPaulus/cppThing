#pragma once


#include "../entity/player.h"

class NetworkClient {
 public:  
  NetworkClient(unsigned int id, unsigned int ip, unsigned int port);
  unsigned int getPort();
  unsigned int getId();
  unsigned int getIp();
  Player *getPlayer();
  void setPlayer(Player* p);
  
 private:
  unsigned int id;
  unsigned int port;
  unsigned int ip;
  Player* player;
  
};
