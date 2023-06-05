#include "NetworkClient.h"

NetworkClient::NetworkClient(unsigned int id, ENetPeer *peer)
{
  _peer = peer;
  _id = id;
  _player = NULL;
}

unsigned int NetworkClient::getId()
{
  return _id;
}

unsigned int NetworkClient::getIp()
{
  return _peer->address.host;
}

unsigned int NetworkClient::getPort()
{
  return _peer->address.port;
}

ENetPeer *NetworkClient::getPeer()
{
  return _peer;
}

void NetworkClient::setPlayer(Player *p)
{
  _player = p;
}

Player *NetworkClient::getPlayer()
{
  return _player;
}
