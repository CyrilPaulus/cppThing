#pragma once


#include "../entity/player.h"
#include <enet/enet.h>

class NetworkClient {
 public:  
  NetworkClient(unsigned int id, ENetPeer* peer);
  unsigned int getPort();
  unsigned int getId();
  unsigned int getIp();
  ENetPeer* getPeer();
  Player *getPlayer();
  void setPlayer(Player* p);
  
 private:
  unsigned int id;
  ENetPeer* peer;  
  Player* player;
  
};
