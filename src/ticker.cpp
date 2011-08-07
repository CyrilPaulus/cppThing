#include "config.h"
#include "ticker.h"

Ticker::Ticker() {
  tickTime = 20;  
}

Ticker::Ticker(float tickTime) {
  this->tickTime = tickTime;    
}

bool Ticker::Tick() {
  if(clock.GetElapsedTime() > tickTime){
    clock.Reset();
    return true;
  }

  return false;
}
