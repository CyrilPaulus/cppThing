#include "../config.h"
#include "player.h"
#include "../utils/vectorutils.h"
#include <algorithm>



Player::Player(ImageManager *imageManager, World* world) : Entity(imageManager) {
  this->world = world;
  sprite->setTexture(*(imageManager->get("player")), true);
  bbox = sf::Vector2f(sprite->getGlobalBounds().width - 2, sprite->getGlobalBounds().height - 2);
  offset = sf::Vector2f(-1 , 0);
  pupil = new sf::Sprite(*(imageManager->get("pupil")));
  colorMask = new sf::Sprite(*(imageManager->get("colorMask")));

  colorMask->setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

  lpOrigin = sf::Vector2f(5, 9);
  rpOrigin = sf::Vector2f(20, 9);

  lpPosition = lpOrigin;
  rpPosition = rpOrigin;

  noclip = false;

  maxWalkSpeed = 100;
  maxFallSpeed = 200;

  speed = sf::Vector2f(0, 0);
  acceleration = sf::Vector2f(350, 350);
  isFlying = false;
  jumpForce = 200;

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
  pupil->setPosition(GetPosition() + lpPosition);
  rt->draw(*(pupil));
  pupil->setPosition(GetPosition() + rpPosition);
  rt->draw(*(pupil));
  colorMask->setPosition(GetPosition());
  rt->draw(*(colorMask));
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
    if(input.Up && !isFlying){
      speed.y -= jumpForce;
      isFlying = true;
    }  
  }

}

void Player::PhysicUpdate(sf::Time frametime) {	
  float seconds = frametime.asSeconds();
  
  if(speed.x > 0 && !moveX) {
    speed.x = std::max(0.F, speed.x - (acceleration.x) * seconds);
  }
  else if(speed.x < 0 && !moveX){
    speed.x = std::min(0.F, speed.x + (acceleration.x) * seconds);
  }    
  
  if(noclip) {
    if(speed.y > 0 && !moveY) {
      speed.y = std::max(0.F, speed.y - (acceleration.y) * seconds);
      }
    else if(speed.y < 0 && !moveY){
      speed.y = std::min(0.F, speed.y + (acceleration.y) * seconds);
    }
  }
  else
    speed.y = std::min(speed.y + (acceleration.y) * seconds, maxFallSpeed);
    
  //Update position x and check for collision
  if(speed.x != 0){
    position.x += speed.x * seconds;
    
    Cube *c = world->GetCollidingCube(GetBbox());
    if( c != NULL){
      if(speed.x < 0)
	position.x = c->GetBbox().left + c->GetBbox().width;
      else
	position.x = c->GetBbox().left - GetBbox().width;
      speed.x = 0;
    }
  }
  
  //Update position with speed and check for collision
  if(speed.y != 0){
    position.y += speed.y * seconds;
    isFlying = true;
    Cube *c = world->GetCollidingCube(GetBbox());
    if( c != NULL){
      if(speed.y < 0)
	position.y = c->GetBbox().top + c->GetBbox().height;
      else{
	position.y = c->GetBbox().top - GetBbox().height;
	isFlying = false;
      }
      speed.y = 0;
    }
  }
}

void Player::Update(sf::Time frametime, Input input) {
  InputUpdate(input);
  PhysicUpdate(frametime);
  /*switch (node->getRole()){
  case eZCom_RoleOwner:
    DoOwner(input, frametime);
    break;
  case eZCom_RoleAuthority:
    DoAuth();
    break;
  case eZCom_RoleProxy:
    DoProxy();
    break;
  default:
    break;
    }*/
    
}

void Player::SetID(int id){
  this->id = id;
}

int Player::GetID(){
  return id;
}

/*
void Player::RegisterZCom(ZCom_Control *control, bool server) {
  node->beginReplicationSetup(5);
  moverep = new ZCom_Replicate_Movement<zFloat, 2>(32, ZCOM_REPFLAG_MOSTRECENT|ZCOM_REPFLAG_SETUPPERSISTS, ZCOM_REPRULE_OWNER_2_AUTH|ZCOM_REPRULE_AUTH_2_PROXY);
  ((ZCom_RSetupMovement<zFloat>*)moverep->getSetup())->setConstantErrorThreshold(0);
  ((ZCom_RSetupMovement<zFloat>*)moverep->getSetup())->setInterpolationTime(50);
  moverep->setTimeScale(0.001);
  
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
*/

void Player::SetColor(sf::Color c){
  colorMask->setColor(c);
}

/*
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

void Player::DoOwner(Input input, unsigned int frametime) {
  while(node && node->checkEventWaiting()) {
    eZCom_Event eve;
    node->getNextEvent(&eve, NULL, NULL);

    if(eve == eZCom_EventRemoved) {
      printf("PLAYER: authority node has been deleted, networkd ID: %d", node->getNetworkID());
      remove = true;
    }
  }

  PhysicUpdate(frametime);
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
  }*/


/*
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
  memcpy(this->position, position, sizeof(this->position));
  memcpy(this->speed, speed, sizeof(this->speed));

  
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
*/
