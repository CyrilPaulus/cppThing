#include "config.h"
#include "ticker.h"

Ticker::Ticker() {
  tickTime = sf::milliseconds(25);
  elapsedTime = sf::milliseconds(0);
}

Ticker::Ticker(sf::Time tickTime) {
  this->tickTime = tickTime;    
}

void Ticker::SetUpdateRate(int updateRate){
  tickTime = sf::milliseconds(1000 / updateRate);
}

bool Ticker::Tick() {
  elapsedTime = clock.getElapsedTime();
  if(elapsedTime >= tickTime){
    clock.restart();
    return true;
  }

  return false;
}

sf::Time Ticker::GetElapsedTime(){
  return elapsedTime;
}
