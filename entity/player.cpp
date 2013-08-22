#include "../config.h"
#include "player.h"
#include "../utils/vectorutils.h"
#include <algorithm>

Player::Player(World* world) : Entity() {
  _world = world;
  _bbox = sf::Vector2f(29, 50);

  _lp_origin = sf::Vector2f(5, 9);
  _rp_origin = sf::Vector2f(20, 9);
  _color = sf::Vector3i(rand() % 255, rand() % 255, rand() % 255);
  _lp_position = _lp_origin;
  _rp_position = _rp_origin;

  _noclip = false;

  _max_walk_speed = 100;
  _max_fall_speed = 200;

  _velocity = sf::Vector2f(0, 0);
  _acceleration = sf::Vector2f(350, 350);
  _is_flying = false;
  _jump_force = 200;
  _acc = sf::seconds(0);
}

Player::~Player(){
}

void Player::setEyesPosition(sf::Vector2f target) {
  float lp_dist = std::min(VectorUtils::distance(target, getPosition() + _lp_origin) / 50, 2.F);
  sf::Vector2f lp_dir = target - (getPosition() + _lp_origin);
  lp_dir = VectorUtils::normalize(lp_dir);
  _lp_position = _lp_origin + (lp_dist * lp_dir);

  float rp_dist = std::min(VectorUtils::distance(target, getPosition() + _rp_origin) / 50, 2.F);
  sf::Vector2f rp_dir = target - (getPosition() + _rp_origin);
  rp_dir = VectorUtils::normalize(rp_dir);
  _rp_position = _rp_origin + (rp_dist * rp_dir);
}

void Player::update(sf::Time frametime, Input input) {
  //First update velocity x
  
  _acc += frametime;
  while(_acc >= sf::milliseconds(GameConstant::SIMULATION_TIME_PER_UPDATE)) {
    _acc -= sf::milliseconds(GameConstant::SIMULATION_TIME_PER_UPDATE);
    float seconds = GameConstant::SIMULATION_TIME_PER_UPDATE / 1000.0;
    //Update velocity X
    if (input.Left) 
      _velocity.x = std::max(_velocity.x - 2 * _acceleration.x * seconds, -_max_walk_speed);
    else if(input.Right) {
      _velocity.x =  std::min(_velocity.x + 2* _acceleration.x * seconds, _max_walk_speed);
    }
  
    //Update velocity Y
    if(_noclip && input.Up)
      _velocity.y = std::max(_velocity.y - 2 * _acceleration.y * seconds, -_max_fall_speed);
    else if(_noclip && input.Down) 
      _velocity.y = std::min(_velocity.y + 2 * _acceleration.y * seconds, _max_fall_speed);
    else if(input.Up && !_is_flying) 
      _velocity.y -= _jump_force;
  
    //Now we can update position

    //First apply some friction
    if(_velocity.x > 0) 
      _velocity.x = std::max(0.F, _velocity.x - (_acceleration.x) * seconds);
    else if(_velocity.x < 0)
      _velocity.x = std::min(0.F, _velocity.x + (_acceleration.x) * seconds);
  
    if(_noclip && _velocity.y > 0) 
      _velocity.y = std::max(0.F, _velocity.y - (_acceleration.y) * seconds);
    else if(_noclip && _velocity.y < 0)
      _velocity.y = std::min(0.F, _velocity.y + (_acceleration.y) * seconds);
    else
      _velocity.y = std::min(_velocity.y + (_acceleration.y) * seconds, _max_fall_speed);
    
    //Update position and check for collision
    if(_velocity.x != 0){
      _position.x += _velocity.x * seconds;
    
      Cube *c = _world->getCollidingCube(getBbox());
      if( c != NULL){
	if(_velocity.x < 0)
	  _position.x = c->getBbox().left + c->getBbox().width;
	else
	  _position.x = c->getBbox().left - getBbox().width;
	_velocity.x = 0;
      }
    }
  
    if(_velocity.y != 0){
      _position.y += _velocity.y * seconds;
      _is_flying = true;
      Cube *c = _world->getCollidingCube(getBbox());
      if( c != NULL){
	if(_velocity.y < 0)
	  _position.y = c->getBbox().top + c->getBbox().height;
	else{
	  _position.y = c->getBbox().top - getBbox().height;
	  _is_flying = false;
	}
	_velocity.y = 0;
      }
    }
  }

}

void Player::setId(int id){
  _id = id;
}

int Player::getId(){
  return _id;
}

void Player::setColor(sf::Vector3i c){
  _color = c;
}

sf::Vector3i Player::getColor(){
  return _color;
}

void Player::setPseudo(std::string pseudo) {
  _pseudo = pseudo;
}

std::string Player::getPseudo() {
  return _pseudo;
}

sf::Vector2f Player::getLeftPupilPosition() {
  return _lp_position;
}

sf::Vector2f Player::getRightPupilPosition() {
  return _rp_position;
}
