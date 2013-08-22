#ifndef _SERVER_H_
#define _SERVER_H_

#include <SFML/Graphics.hpp>


#include <list>
#include <set>
#include "ticker.h"
#include "imageManager.h"
#include "entity/world.h"
#include "network/NetworkClient.h"
#include "network/packet.h"
#include <enet/enet.h>

class Server {
 public:
  Server();
  ~Server();
  void run();
  void stop();
  void update(sf::Time frametime);
  void init();
  void setPort(int port);

 private:
  void addClient(ENetPeer* peer);
  void removeClient(unsigned int ip, unsigned int port);
  NetworkClient* getClientByPeer(ENetPeer* peer);
  void handlePacket(sf::Packet p, ENetPeer* peer);
  void broadcastReliable(Packet* p);
  void broadcast(Packet* p);
  void sendReliable(ENetPeer* peer, Packet *p);
  void sendFullWorldUpdate(ENetPeer* peer);
  std::string getUniquePseudo(std::string p);
  
  bool _running;
  Ticker* _ticker;
  World* _world;
  int _port;
  int _max_client;
  int _last_client_id;
  std::list<NetworkClient*> _clients;
  ENetHost* _server;
  std::set<std::string> _client_names;
};

#endif /* _SERVER_H_ */
