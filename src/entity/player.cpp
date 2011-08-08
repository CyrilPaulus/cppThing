#include "../config.h"
#include "player.h"
#include "../utils/vectorutils.h"
#include <algorithm>

Player::Player(World *world, ImageManager *imageManager) : Entity(imageManager) {
  this->world = world;
  sprite->SetImage(*(imageManager->get("player")), true);
  bbox = sf::Vector2f(sprite->GetSize().x - 2, sprite->GetSize().y - 2);
  offset = sf::Vector2f(-1 , 0);
  pupil = new sf::Sprite(*(imageManager->get("pupil")));
  colorMask = new sf::Sprite(*(imageManager->get("colorMask")));
  
  //TODO Random color generation
  
  lpOrigin = sf::Vector2f(5, 9);
  rpOrigin = sf::Vector2f(20, 9);
  
  lpPosition = lpOrigin;
  rpPosition = rpOrigin; 
}

Player::~Player(){
  delete pupil;
  delete colorMask;
}

void Player::Draw(sf::RenderTarget * rt){
  Entity::Draw(rt);
  pupil->SetPosition(position + lpPosition);
  rt->Draw(*(pupil));
  pupil->SetPosition(position + rpPosition);
  rt->Draw(*(pupil));
  colorMask->SetPosition(position);
  rt->Draw(*(colorMask));
}

void Player::SetEyesPosition(sf::Vector2f target){
  float lpDist = std::min(VectorUtils::Distance(target, position + lpOrigin) / 50, 2.F);
  sf::Vector2f lpDir = target - (position + lpOrigin);
  lpDir = VectorUtils::Normalize(lpDir);
  lpPosition = lpOrigin + (lpDist * lpDir);

  float rpDist = std::min(VectorUtils::Distance(target, position + rpOrigin) / 50, 2.F);
  sf::Vector2f rpDir = target - (position + rpOrigin);
  rpDir = VectorUtils::Normalize(rpDir);
  rpPosition = rpOrigin + (rpDist * rpDir);
}
