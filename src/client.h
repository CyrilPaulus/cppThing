#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <SFML/Graphics.hpp>
#include <zoidcom.h>
#include "ticker.h"
#include "imageManager.h"
#include "mouse.h"

#include "entity/world.h"
#include "entity/player.h"

class Client : public ZCom_Control{
 public:
  Client(sf::RenderWindow *window, ImageManager *imageManager, ZoidCom*);
  ~Client();
  int Run();
  void Connect();
//zoidcom callbacks
  void ZCom_cbConnectResult( ZCom_ConnID id, eZCom_ConnectResult result, ZCom_BitStream &reply );
  bool ZCom_cbConnectionRequest( ZCom_ConnID  id, ZCom_BitStream &request, ZCom_BitStream &reply ){return false;}
  void ZCom_cbConnectionSpawned( ZCom_ConnID id ) {}
  void ZCom_cbConnectionClosed( ZCom_ConnID id, eZCom_CloseReason reason, ZCom_BitStream &reasondata );
  bool ZCom_cbZoidRequest( ZCom_ConnID id, zU8 requested_level, ZCom_BitStream &reason ) {return false;}
  void ZCom_cbZoidResult( ZCom_ConnID id, eZCom_ZoidResult result, zU8 new_level, ZCom_BitStream &reason );
  void ZCom_cbNodeRequest_Dynamic( ZCom_ConnID id, ZCom_ClassID requested_class, ZCom_BitStream *announcedata,
                                   eZCom_NodeRole role, ZCom_NodeID net_id );
  void ZCom_cbNodeRequest_Tag( ZCom_ConnID id, ZCom_ClassID requested_class, ZCom_BitStream *announcedata, 
                               eZCom_NodeRole role, zU32 tag ) {}
  void ZCom_cbDataReceived( ZCom_ConnID id, ZCom_BitStream &data ) {}
  bool ZCom_cbDiscoverRequest( const ZCom_Address &addr, 
                               ZCom_BitStream &request, ZCom_BitStream &reply ) {return false;}
  void ZCom_cbDiscovered( const ZCom_Address &addr, ZCom_BitStream &reply )  {}
 private:

  void Update(unsigned int);
  void Draw();

  std::string pseudo;
  sf::RenderWindow *window;
  sf::RenderTexture *worldDisplay;
  Mouse *mouse;
  ImageManager *imageManager;
  Ticker *ticker;
  bool running;
  ZoidCom* zcom;
  ZCom_ConnID clientId;
  ZCom_ConnID serverId;
  void HandleEvent(sf::Event event);
  void OnClose();
  void OnMouseButtonPressed(sf::Event event);
  void OnMouseButtonReleased(sf::Event event);
  void OnMouseWheelMoved(sf::Event event);
  void OnResized(sf::Event event);

  void UpdateView();
  float zoom;
  int cubeType;
  Cube* displayCube;

  World *world;

  bool addCube;
  bool removeCube;
  Player *player;
  
};

#endif /* _CLIENT_H_ */
