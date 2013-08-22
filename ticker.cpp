#include "config.h"
#include "ticker.h"

Ticker::Ticker() {
  _tick_time = sf::milliseconds(25);
  _elapsed_time = sf::milliseconds(0);
}

Ticker::Ticker(sf::Time _tick_time) {
  _tick_time = _tick_time;    
}

void Ticker::setUpdateRate(int update_rate){
  _tick_time = sf::milliseconds(1000 / update_rate);
}

bool Ticker::tick() {
  _elapsed_time = _clock.getElapsedTime();
  if(_elapsed_time >= _tick_time){
    _clock.restart();
    return true;
  }

  return false;
}

sf::Time Ticker::getElapsedTime(){
  return _elapsed_time;
}
