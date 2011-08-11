#include "../config.h"
#include "player.h"
#include "../utils/vectorutils.h"
#include <algorithm>

ZCom_ClassID Player::netClassServerId = ZCom_Invalid_ID;
ZCom_ClassID Player::netClassClientId = ZCom_Invalid_ID;

Player::Player(ImageManager *imageManager, World* world) : Entity(imageManager) {
  this->world = world;
  sprite->SetTexture(*(imageManager->get("player")), true);
  bbox = sf::Vector2f(sprite->GetSize().x - 2, sprite->GetSize().y - 2);
  offset = sf::Vector2f(-1 , 0);
  pupil = new sf::Sprite(*(imageManager->get("pupil")));
  colorMask = new sf::Sprite(*(imageManager->get("colorMask")));

  colorMask->SetColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

  lpOrigin = sf::Vector2f(5, 9);
  rpOrigin = sf::Vector2f(20, 9);

  lpPosition = lpOrigin;
  rpPosition = rpOrigin;

  noclip = false;

  maxWalkSpeed = 100;
  maxFallSpeed = 100;

  speed = sf::Vector2f(0, 0);
  acceleration = sf::Vector2f(350, 350);
  isFlying = false;
  jumpForce = 200;

  accumTime = 0;
  currentTime = 0;

  moveX = false;
  moveY = false;
}

Player::~Player(){
  if(pupil)
    delete pupil;
  if(colorMask)
    delete colorMask;
}

void Player::Draw(sf::RenderTarget * rt) {
  Entity::Draw(rt);
  pupil->SetPosition(position + lpPosition);
  rt->Draw(*(pupil));
  pupil->SetPosition(position + rpPosition);
  rt->Draw(*(pupil));
  colorMask->SetPosition(position);
  rt->Draw(*(colorMask));
}

void Player::SetEyesPosition(sf::Vector2f target) {
  float lpDist = std::min(VectorUtils::Distance(target, position + lpOrigin) / 50, 2.F);
  sf::Vector2f lpDir = target - (position + lpOrigin);
  lpDir = VectorUtils::Normalize(lpDir);
  lpPosition = lpOrigin + (lpDist * lpDir);

  float rpDist = std::min(VectorUtils::Distance(target, position + rpOrigin) / 50, 2.F);
  sf::Vector2f rpDir = target - (position + rpOrigin);
  rpDir = VectorUtils::Normalize(rpDir);
  rpPosition = rpOrigin + (rpDist * rpDir);
}

void Player::InputUpdate(unsigned int frametime, Input input) {
  //First update speed x
  moveX = false;
  moveY = false;
  float frameSec = (GameConstant::SIMULATION_TIME_PER_UPDATE / 1000.f);
  if (input.Left && speed.x > -maxWalkSpeed) {
    speed.x -= acceleration.x * frameSec;
    moveX = true;
  }
  else if(input.Right && speed.x < maxWalkSpeed) {
    speed.x +=  acceleration.x * frameSec;
    moveX = true;
  }
  
  //Update speed Y
  if(noclip){
    if (input.Up && speed.y > -maxFallSpeed) {
      speed.y -= (acceleration.y) * frameSec;
      moveY = true;
    }
    else if(input.Down && speed.y < maxFallSpeed) {
      speed.y += (acceleration.y) * frameSec;
      moveY = true;
    }    
  }else {
    if(input.Up && !isFlying)
      speed.y -= jumpForce;    
  }

}

void Player::PhysicUpdate(unsigned int frametime) {
  float frameSec = (GameConstant::SIMULATION_TIME_PER_UPDATE) / 1000.f;
  accumTime += frametime;
  while(accumTime > 0 && accumTime >= GameConstant::SIMULATION_TIME_PER_UPDATE) {
    accumTime -= GameConstant::SIMULATION_TIME_PER_UPDATE;
    
    if(speed.x > 0 && !moveX) {
      speed.x = std::max(0.F, speed.x - (acceleration.x) * frameSec);
    }
    else if(speed.x < 0 && !moveX){
      speed.x = std::min(0.F, speed.x + (acceleration.x) * frameSec);
    }    
    
    if(noclip) {
      if(speed.y > 0 && !moveY) {
	speed.y = std::max(0.F, speed.y - (acceleration.y) * frameSec);
      }
      else if(speed.y < 0 && !moveY){
	speed.y = std::min(0.F, speed.y + (acceleration.y) * frameSec);
      }
    }
    else
      speed.y += std::min((acceleration.y) * frameSec, maxFallSpeed);
    
    //Update position x and check for collision
    if(speed.x != 0){
      position.x += speed.x * frameSec;
      
      Cube *c = world->GetCollidingCube(GetBbox());
      if( c != NULL){
	if(speed.x < 0)
	  position.x = c->GetBbox().Left + c->GetBbox().Width;
	else
	  position.x = c->GetBbox().Left - GetBbox().Width;
	speed.x = 0;
      }
    }
    
    //Update position with speed and check for collision
    if(speed.y != 0){
      position.y += speed.y * frameSec;
      isFlying = true;
      Cube *c = world->GetCollidingCube(GetBbox());
      if( c != NULL){
	if(speed.y < 0)
	  position.y = c->GetBbox().Top + c->GetBbox().Height;
	else{
	  position.y = c->GetBbox().Top - GetBbox().Height;
	  isFlying = false;
	}
	speed.y = 0;
      }
    }
  }
}

void Player::Update(unsigned int frametime, Input input) {
  InputUpdate(frametime, input);
  PhysicUpdate(frametime);
}
void Player::RegisterClass(ZCom_Control * control, bool server){
  if(server)
    Player::netClassServerId = control->ZCom_registerClass("player", ZCOM_CLASSFLAG_ANNOUNCEDATA);
  else
    Player::netClassClientId = control->ZCom_registerClass("player", ZCOM_CLASSFLAG_ANNOUNCEDATA);
}

void Player::SetID(ZCom_ConnID id){
  this->id = id;
}

ZCom_ConnID Player::GetID(){
  return id;
}

void Player::RegisterZCom(ZCom_Control *control, bool server) {
  node->beginReplicationSetup(6);
  node->addReplicationFloat(&position.x, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_AUTH_2_ALL);
  node->addReplicationFloat(&position.y, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_AUTH_2_ALL);
  node->addReplicationFloat(&lpPosition.x, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_AUTH_2_ALL);
  node->addReplicationFloat(&lpPosition.y, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_AUTH_2_ALL);
  node->addReplicationFloat(&rpPosition.x, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_AUTH_2_ALL);
  node->addReplicationFloat(&rpPosition.y, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_AUTH_2_ALL);
  node->endReplicationSetup();
  node->registerNodeDynamic(GetClass(server), control);
  if(server){
    ZCom_BitStream *adata = new ZCom_BitStream();
    adata->addInt(id, 32);
    adata->addFloat(position.x, 23);
    adata->addFloat(position.y, 23);
    adata->addInt(colorMask->GetColor().r, 8);
    adata->addInt(colorMask->GetColor().g, 8);
    adata->addInt(colorMask->GetColor().b, 8);
    node->setAnnounceData(adata);
  }
}

ZCom_ClassID Player::GetClass(bool server) {
  if(server)
    return(Player::netClassServerId);
  else
    return(Player::netClassClientId);
}

void Player::SetColor(sf::Color c){
  colorMask->SetColor(c);
}
