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

  speed[0] = speed[1]  = 0;
  acceleration = sf::Vector2f(350, 350);
  isFlying = false;
  jumpForce = 200;

  accumTime = 0;
  currentTime = 0;

  moveX = false;
  moveY = false;

  lastInputSent = Input();
}

Player::~Player(){
  if(pupil)
    delete pupil;
  if(colorMask)
    delete colorMask;
}

void Player::Draw(sf::RenderTarget * rt) {
  Entity::Draw(rt);
  pupil->SetPosition(GetPosition() + lpPosition);
  rt->Draw(*(pupil));
  pupil->SetPosition(GetPosition() + rpPosition);
  rt->Draw(*(pupil));
  colorMask->SetPosition(GetPosition());
  rt->Draw(*(colorMask));
}

void Player::SetEyesPosition(sf::Vector2f target) {
  float lpDist = std::min(VectorUtils::Distance(target, GetPosition() + lpOrigin) / 50, 2.F);
  sf::Vector2f lpDir = target - (GetPosition() + lpOrigin);
  lpDir = VectorUtils::Normalize(lpDir);
  lpPosition = lpOrigin + (lpDist * lpDir);

  float rpDist = std::min(VectorUtils::Distance(target, GetPosition() + rpOrigin) / 50, 2.F);
  sf::Vector2f rpDir = target - (GetPosition() + rpOrigin);
  rpDir = VectorUtils::Normalize(rpDir);
  rpPosition = rpOrigin + (rpDist * rpDir);
}

void Player::InputUpdate(Input input) {
  //First update speed x
  moveX = false;
  moveY = false;
  float frameSec = (GameConstant::SIMULATION_TIME_PER_UPDATE / 1000.f);
  if (input.Left && speed[0] > -maxWalkSpeed) {
    speed[0] -= acceleration.x * frameSec;
    moveX = true;
  }
  else if(input.Right && speed[0] < maxWalkSpeed) {
    speed[0] +=  acceleration.x * frameSec;
    moveX = true;
  }
  
  //Update speed Y
  if(noclip){
    if (input.Up && speed[1] > -maxFallSpeed) {
      speed[1] -= (acceleration.y) * frameSec;
      moveY = true;
    }
    else if(input.Down && speed[1] < maxFallSpeed) {
      speed[1] += (acceleration.y) * frameSec;
      moveY = true;
    }    
  }else {
    if(input.Up && !isFlying){
      speed[1] -= jumpForce;
      isFlying = true;
    }  
  }

}

void Player::PhysicUpdate(unsigned int frametime) {
  float frameSec = (GameConstant::SIMULATION_TIME_PER_UPDATE) / 1000.f;
  accumTime += frametime;
  while(accumTime > 0 && accumTime >= GameConstant::SIMULATION_TIME_PER_UPDATE) {
    accumTime -= GameConstant::SIMULATION_TIME_PER_UPDATE;
    
    if(speed[0] > 0 && !moveX) {
      speed[0] = std::max(0.F, speed[0] - (acceleration.x) * frameSec);
    }
    else if(speed[0] < 0 && !moveX){
      speed[0] = std::min(0.F, speed[0] + (acceleration.x) * frameSec);
    }    
    
    if(noclip) {
      if(speed[1] > 0 && !moveY) {
	speed[1] = std::max(0.F, speed[1] - (acceleration.y) * frameSec);
      }
      else if(speed[1] < 0 && !moveY){
	speed[1] = std::min(0.F, speed[1] + (acceleration.y) * frameSec);
      }
    }
    else
      speed[1] += std::min((acceleration.y) * frameSec, maxFallSpeed);
    
    //Update position x and check for collision
    if(speed[0] != 0){
      position[0] += speed[0] * frameSec;
      
      Cube *c = world->GetCollidingCube(GetBbox());
      if( c != NULL){
	if(speed[0] < 0)
	  position[0] = c->GetBbox().Left + c->GetBbox().Width;
	else
	  position[0] = c->GetBbox().Left - GetBbox().Width;
	speed[0] = 0;
      }
    }
    
    //Update position with speed and check for collision
    if(speed[1] != 0){
      position[1] += speed[1] * frameSec;
      isFlying = true;
      Cube *c = world->GetCollidingCube(GetBbox());
      if( c != NULL){
	if(speed[1] < 0)
	  position[1] = c->GetBbox().Top + c->GetBbox().Height;
	else{
	  position[1] = c->GetBbox().Top - GetBbox().Height;
	  isFlying = false;
	}
	speed[1] = 0;
      }
    }
  }
}

void Player::Update(unsigned int frametime, Input input) {
  //InputUpdate(input);
  //PhysicUpdate(frametime);
  switch (node->getRole()){
  case eZCom_RoleOwner:
    DoOwner(input);
    break;
  case eZCom_RoleAuthority:
    DoAuth();
    break;
  case eZCom_RoleProxy:
    DoProxy();
    break;
  default:
    break;
  }
    
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
  node->beginReplicationSetup(5);
  moverep = new ZCom_Replicate_Movement<zFloat, 2>(32, ZCOM_REPFLAG_MOSTRECENT|ZCOM_REPFLAG_SETUPPERSISTS, ZCOM_REPRULE_OWNER_2_AUTH|ZCOM_REPRULE_AUTH_2_PROXY);
  ((ZCom_RSetupMovement<zFloat>*)moverep->getSetup())->setConstantErrorThreshold(0);
  ((ZCom_RSetupMovement<zFloat>*)moverep->getSetup())->setInterpolationTime(50);
  moverep->setTimeScale((float)GameConstant::UPDATE_RATE/1000.0f);
  
  node->addReplicator(moverep, true);
  moverep->setUpdateListener(this);
  moverep->updateState((zFloat*) &position[0], (zFloat*) &speed[0], NULL, false);
  ZCom_BitStream *bs = new ZCom_BitStream(1);
  lastInputSent.Pack(*bs);
  moverep->updateInput(position, bs);
  node->addReplicationFloat(&lpPosition.x, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_OWNER_2_AUTH | ZCOM_REPRULE_AUTH_2_PROXY);
  node->addReplicationFloat(&lpPosition.y, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_OWNER_2_AUTH | ZCOM_REPRULE_AUTH_2_PROXY);
  node->addReplicationFloat(&rpPosition.x, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_OWNER_2_AUTH | ZCOM_REPRULE_AUTH_2_PROXY);
  node->addReplicationFloat(&rpPosition.y, 23, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_OWNER_2_AUTH | ZCOM_REPRULE_AUTH_2_PROXY);
  node->endReplicationSetup();
  
  node->registerNodeDynamic(GetClass(server), control);
  node->setEventNotification(true, false);
  node->applyForZoidLevel(1);
  if(server) {
    ZCom_BitStream *adata = new ZCom_BitStream();
    adata->addInt(id, 32);
    adata->addFloat(position[0], 23);
    adata->addFloat(position[1], 23);
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

void Player::DoAuth() {
  while (node && node->checkEventWaiting()) {
    eZCom_Event eve;
    ZCom_ConnID connid;
    eZCom_NodeRole remoterole;
    node->getNextEvent(&eve, &remoterole, &connid);

    if(eve == eZCom_EventInit && node->getRole() == eZCom_RoleAuthority) {
      printf("PLAYER: authority node got init for node: %d role: %d", node->getNetworkID(), remoterole);
    }  
  }  
}

void Player::DoOwner(Input input) {
  while(node && node->checkEventWaiting()) {
    eZCom_Event eve;
    node->getNextEvent(&eve, NULL, NULL);

    if(eve == eZCom_EventRemoved) {
      printf("PLAYER: authority node has been deleted, networkd ID: %d", node->getNetworkID());
      remove = true;
    }
  }

  PhysicUpdate(GameConstant::SIMULATION_TIME_PER_UPDATE);
  if( input == lastInputSent) {
    moverep->updateInput(position, NULL);
  } else {
    ZCom_BitStream *bs = new ZCom_BitStream(1);
    input.Pack(*bs);
    moverep->updateInput(position, bs);
  }
}

void Player::DoProxy() {
  while (node && node->checkEventWaiting()) {
    eZCom_Event eve;
    node->getNextEvent(&eve, NULL, NULL);
    
    if(eve == eZCom_EventRemoved) {
      remove = true;
    }
  }
  moverep->getExtrapolatedPosition(0, position);
  moverep->getExtrapolatedVelocity(0, speed);
}


void Player::inputUpdated(ZCom_BitStream& inputstream, bool inputchanged, zU32 clientTime, zU32 estimatedtimesent) {
  Input in;
  if(inputchanged) {
    in.Unpack(inputstream);
    lastInputReceived = in;
  }
  else
    in = lastInputReceived;

  if(clientTime <= currentTime)
    return;

  if(currentTime > 0)
    PhysicUpdate(clientTime - currentTime);

  moverep->updateState(position, speed, NULL, false);
  currentTime = clientTime;
  InputUpdate(in);
}

void Player::inputSent(ZCom_BitStream& inputstream) {
  Input in;
  in.Unpack(inputstream);
  
  InputUpdate(in);
  lastInputSent = in;
}

void Player::correctionReceived(zFloat *position, zFloat *speed, zFloat *acc, bool teleport, zU32 estimatedTimeSent) {
  Input input;
  zU32 time, currentTimeBis = 0;
  ZCom_BitStream* inputStream = NULL;
  void *iter = NULL;

  accumTime = 0;
  memcpy(this->position, position, sizeof(position));
  memcpy(this->speed, speed, sizeof(speed));
  
  while (true) {
    inputStream = moverep->getNextInputHistoryEntry(&time, &iter);
    
    if (currentTimeBis > 0 && time - currentTimeBis > 0)
      PhysicUpdate(time - currentTimeBis);

    //No more move for correction
    if(!inputStream)
      break;

    input.Unpack(*inputStream);
    InputUpdate(input);

    currentTimeBis = time;
  }
}
