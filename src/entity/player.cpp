#include "../config.h"
#include "player.h"
#include "../utils/vectorutils.h"
#include <algorithm>



Player::Player(ImageManager *imageManager, World* world) : Entity(imageManager) {
  this->world = world;
  sprite->setTexture(*(imageManager->get("player")), true);
  bbox = sf::Vector2f(sprite->getGlobalBounds().width, sprite->getGlobalBounds().height);
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

  velocity = sf::Vector2f(0, 0);
  acceleration = sf::Vector2f(350, 350);
  isFlying = false;
  jumpForce = 200;
  acc = sf::seconds(0);
}

Player::~Player(){
  if(pupil)
    delete pupil;
  if(colorMask)
    delete colorMask;
}

void Player::draw(sf::RenderTarget * rt) {
  Entity::draw(rt);
  pupil->setPosition(getPosition() + lpPosition);
  rt->draw(*(pupil));
  pupil->setPosition(getPosition() + rpPosition);
  rt->draw(*(pupil));
  colorMask->setPosition(getPosition());
  rt->draw(*(colorMask));
}

void Player::setEyesPosition(sf::Vector2f target) {
  float lpDist = std::min(VectorUtils::Distance(target, getPosition() + lpOrigin) / 50, 2.F);
  sf::Vector2f lpDir = target - (getPosition() + lpOrigin);
  lpDir = VectorUtils::Normalize(lpDir);
  lpPosition = lpOrigin + (lpDist * lpDir);

  float rpDist = std::min(VectorUtils::Distance(target, getPosition() + rpOrigin) / 50, 2.F);
  sf::Vector2f rpDir = target - (getPosition() + rpOrigin);
  rpDir = VectorUtils::Normalize(rpDir);
  rpPosition = rpOrigin + (rpDist * rpDir);
}

void Player::update(sf::Time frametime, Input input) {
  //First update velocity x
  
  acc += frametime;
  while(acc >= sf::milliseconds(GameConstant::SIMULATION_TIME_PER_UPDATE)) {
    acc -= sf::milliseconds(GameConstant::SIMULATION_TIME_PER_UPDATE);
    float seconds = GameConstant::SIMULATION_TIME_PER_UPDATE / 1000.0;
    //Update velocity X
    if (input.Left) 
      velocity.x = std::max(velocity.x - 2 * acceleration.x * seconds, -maxWalkSpeed);
    else if(input.Right) {
      velocity.x =  std::min(velocity.x + 2* acceleration.x * seconds, maxWalkSpeed);
    }
  
    //Update velocity Y
    if(noclip && input.Up)
      velocity.y = std::max(velocity.y - 2 * acceleration.y * seconds, -maxFallSpeed);
    else if(noclip && input.Down) 
      velocity.y = std::min(velocity.y + 2 * acceleration.y * seconds, maxFallSpeed);
    else if(input.Up && !isFlying) 
      velocity.y -= jumpForce;
  

    //Now we can update position

    //First apply some friction
    if(velocity.x > 0) 
      velocity.x = std::max(0.F, velocity.x - (acceleration.x) * seconds);
    else if(velocity.x < 0)
      velocity.x = std::min(0.F, velocity.x + (acceleration.x) * seconds);
  
    if(noclip && velocity.y > 0) 
      velocity.y = std::max(0.F, velocity.y - (acceleration.y) * seconds);
    else if(noclip && velocity.y < 0)
      velocity.y = std::min(0.F, velocity.y + (acceleration.y) * seconds);
    else
      velocity.y = std::min(velocity.y + (acceleration.y) * seconds, maxFallSpeed);
    
    //Update position and check for collision
    if(velocity.x != 0){
      position.x += velocity.x * seconds;
    
      Cube *c = world->getCollidingCube(getBbox());
      if( c != NULL){
	if(velocity.x < 0)
	  position.x = c->getBbox().left + c->getBbox().width;
	else
	  position.x = c->getBbox().left - getBbox().width;
	velocity.x = 0;
      }
    }
  
    if(velocity.y != 0){
      position.y += velocity.y * seconds;
      isFlying = true;
      Cube *c = world->getCollidingCube(getBbox());
      if( c != NULL){
	if(velocity.y < 0)
	  position.y = c->getBbox().top + c->getBbox().height;
	else{
	  position.y = c->getBbox().top - getBbox().height;
	  isFlying = false;
	}
	velocity.y = 0;
      }
    }
  }

}

/*void Player::Update(sf::Time frametime, Input input) {
  InputUpdate(input);
  PhysicUpdate(frametime);
  switch (node->getRole()){
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
    }
    
}*/

void Player::setId(int id){
  this->id = id;
}

int Player::getId(){
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

void Player::setColor(sf::Color c){
  colorMask->setColor(c);
}

sf::Color Player::getColor(){
  return colorMask->getColor();
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

void Player::setPseudo(std::string pseudo) {
  this->pseudo = pseudo;
}

std::string Player::getPseudo() {
  return pseudo;
}
