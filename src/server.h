#ifndef _SERVER_H_
#define _SERVER_H_

#include <zoidcom.h>
#include <SFML/Graphics.hpp>

#include "ticker.h"
#include "imageManager.h"
#include "entity/world.h"

class Server : public ZCom_Control {
 public:
  Server(ImageManager *);
  ~Server();
  void Run();
  void Stop();
  void Update(float);
  void Init();
  //zoidcom callbacks
  void ZCom_cbConnectResult( ZCom_ConnID id, eZCom_ConnectResult result, ZCom_BitStream &reply ) {}
  bool ZCom_cbConnectionRequest( ZCom_ConnID  id, ZCom_BitStream &request, ZCom_BitStream &reply );
  void ZCom_cbConnectionSpawned( ZCom_ConnID id );
  void ZCom_cbConnectionClosed( ZCom_ConnID id, eZCom_CloseReason reason, ZCom_BitStream &reasondata );
  bool ZCom_cbZoidRequest( ZCom_ConnID id, zU8 requested_level, ZCom_BitStream &reason ) {return false;}
  void ZCom_cbZoidResult( ZCom_ConnID id, eZCom_ZoidResult result, zU8 new_level, ZCom_BitStream &reason ) {}
  void ZCom_cbNodeRequest_Dynamic( ZCom_ConnID id, ZCom_ClassID requested_class, ZCom_BitStream *announcedata,
                                   eZCom_NodeRole role, ZCom_NodeID net_id ) {}
  void ZCom_cbNodeRequest_Tag( ZCom_ConnID id, ZCom_ClassID requested_class, ZCom_BitStream *announcedata, 
                               eZCom_NodeRole role, zU32 tag ) {}
  void ZCom_cbDataReceived( ZCom_ConnID id, ZCom_BitStream &data );
  bool ZCom_cbDiscoverRequest( const ZCom_Address &addr, 
                               ZCom_BitStream &request, ZCom_BitStream &reply ) {return false;}
  void ZCom_cbDiscovered( const ZCom_Address &addr, ZCom_BitStream &reply )  {}
  
 private:
  bool running;
  ImageManager *imageManager;
  Ticker *ticker;
  World *world;
  
};

#endif /* _SERVER_H_ */
