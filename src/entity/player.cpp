#include "../config.h"
#include "player.h"
#include "../utils/vectorutils.h"
#include <algorithm>

Player::Player(World *world, ImageManager *imageManager) : Entity(imageManager) {
  this->world = world;
  sprite->SetTexture(*(imageManager->get("player")), true);
  bbox = sf::Vector2f(sprite->GetSize().x - 2, sprite->GetSize().y - 2);
  offset = sf::Vector2f(-1 , 0);
  pupil = new sf::Sprite(*(imageManager->get("pupil")));
  colorMask = new sf::Sprite(*(imageManager->get("colorMask")));

  //TODO Random color generation
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
}

Player::~Player(){
  delete pupil;
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

void Player::Update(float frametime, Input input) {
  //First update speed x

  if (input.Left && speed.x > -maxWalkSpeed) {
    speed.x -= (acceleration.x * frametime);
  }
  else if(input.Right && speed.x < maxWalkSpeed) {
    speed.x += (acceleration.x * frametime);
  }
  else if(speed.x > 0) {
    speed.x = std::max(0.F, speed.x - (acceleration.x * frametime));
  }
  else if(speed.x < 0){
    speed.x = std::min(0.F, speed.x + (acceleration.x * frametime));
  }

  //Update position x and check for collision
  if(speed.x != 0){
    position.x += speed.x * frametime;

    Cube *c = world->GetCollidingCube(GetBbox());
    if( c != NULL){
      if(speed.x < 0)
	position.x = c->GetBbox().Left + c->GetBbox().Width;
      else
	position.x = c->GetBbox().Left - GetBbox().Width;
       speed.x = 0;
    }
  }

  //Update speed Y
  if(noclip){
    if (input.Up && speed.y > -maxFallSpeed) {
      speed.y -= (acceleration.y * frametime);
    }
    else if(input.Down && speed.y < maxFallSpeed) {
      speed.y += (acceleration.y * frametime);
    }
    else if(speed.y > 0) {
      speed.y = std::max(0.F, speed.y - (acceleration.y * frametime));
    }
    else if(speed.y < 0){
      speed.y = std::min(0.F, speed.y + (acceleration.y * frametime));
    }
  }else {
    if(input.Up && !isFlying)
      speed.y -= jumpForce;
    speed.y += (acceleration.y * frametime);
  }

  //Update position with speed and check for collision
  if(speed.y != 0){
    position.y += speed.y * frametime;
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
