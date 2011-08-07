#ifndef _TICKER_H_
#define _TICKER_H_

#include <SFML/System/Clock.hpp>

class Ticker{
 public:
  Ticker();
  Ticker(float tickTime);
  bool Tick();
  float GetElapsedSeconds();
  
 private:
  sf::Clock clock;
  float tickTime;
  uint elapsedTime;
};

#endif /* _TICKER_H_ */
