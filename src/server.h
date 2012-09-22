#ifndef _SERVER_H_
#define _SERVER_H_

#include <SFML/Graphics.hpp>


#include <list>
#include "ticker.h"
#include "imageManager.h"
#include "entity/world.h"
#include "network/NetworkClient.h"
#include "network/packet.h"
#include <enet/enet.h>

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
  void addClient(ENetPeer* peer);
  void removeClient(unsigned int ip, unsigned int port);
  void handlePacket(sf::Packet p);
  void broadcastReliable(Packet* p);
  void sendReliable(ENetPeer* peer, Packet *p);
  void sendFullWorld(ENetPeer* peer);
  bool running;
  ImageManager *imageManager;
  Ticker *ticker;
  World *world;
  int port;
  int maxClient;
  int lastClientID;
  std::list<NetworkClient> clients;
  ENetHost* server;
};

#endif /* _SERVER_H_ */
