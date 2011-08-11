#ifndef _TICKER_H_
#define _TICKER_H_

#include <SFML/System/Clock.hpp>

class Ticker{
 public:
  Ticker();
  Ticker(float tickTime);
  bool Tick();
  unsigned int GetElapsedMilliSeconds();

 private:
  sf::Clock clock;
  float tickTime;
  unsigned int elapsedTime;
};

#endif /* _TICKER_H_ */
