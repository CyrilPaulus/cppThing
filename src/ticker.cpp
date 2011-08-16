#include "config.h"
#include "ticker.h"

Ticker::Ticker() {
  tickTime = 25;
  elapsedTime = 0;
}

Ticker::Ticker(float tickTime) {
  this->tickTime = tickTime;    
}

void Ticker::SetUpdateRate(int updateRate){
	tickTime = 1000 / updateRate;
}

bool Ticker::Tick() {
  elapsedTime = clock.GetElapsedTime();
  if(elapsedTime >= tickTime){
    clock.Reset();
    return true;
  }

  return false;
}

unsigned int Ticker::GetElapsedMilliSeconds(){
  return elapsedTime;
}
