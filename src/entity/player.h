#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include "entity.h"
#include "world.h"
#include "../input/input.h"

class World;

class Player : public Entity, public ZCom_MoveUpdateListener<zFloat> {
 public:
  Player(ImageManager*, World*);
  ~Player();
  void Draw(sf::RenderTarget *);
  void SetEyesPosition(sf::Vector2f);
  void Update(unsigned int, Input);
  void InputUpdate(Input);
  void PhysicUpdate(unsigned int);
  void SetColor(sf::Color);
  
  static void RegisterClass(ZCom_Control * control, bool server);
  void RegisterZCom(ZCom_Control *, bool);
  void SetID(ZCom_ConnID);
  ZCom_ConnID GetID();
  static ZCom_ClassID GetClass(bool server);

 private:
  void DoOwner(Input input, unsigned int frametime);
  void DoProxy();
  void DoAuth();
  
  static ZCom_ClassID netClassServerId;
  static ZCom_ClassID netClassClientId;

  ZCom_ConnID id;
  World* world;
  ZCom_Replicate_Movement<zFloat, 2> *moverep;

  bool moveX;
  bool moveY;

  bool noclip;
  bool isFlying;
  sf::Sprite *pupil;
  sf::Sprite *colorMask;

  sf::Vector2f lpOrigin;
  sf::Vector2f lpPosition;
  sf::Vector2f rpOrigin;
  sf::Vector2f rpPosition;

  float maxWalkSpeed;
  float maxFallSpeed;
  float jumpForce;

  zFloat speed[2];
  sf::Vector2f acceleration;
  zU32 currentTime;
  zU32 accumTime;
  Input lastInputSent;
  Input lastInputReceived;
  bool updateOut;
  bool updateIn;

  void inputUpdated(ZCom_BitStream&, bool, zU32, zU32);
  void inputSent(ZCom_BitStream&);
  void correctionReceived(zFloat*, zFloat*, zFloat*, bool, zU32);
  void updateReceived(ZCom_BitStream&, zFloat*, zFloat*, zFloat*, zU32) {};
};

#endif /* _PLAYER_H_ */
