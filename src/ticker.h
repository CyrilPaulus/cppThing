#ifndef _TICKER_H_
#define _TICKER_H_

#include <SFML/System/Clock.hpp>

class Ticker{
 public:
  Ticker();
  Ticker(sf::Time tickTime);
  bool Tick();
  sf::Time GetElapsedTime();
  void SetUpdateRate(int);

 private:
  sf::Clock clock;
  sf::Time tickTime;
  sf::Time elapsedTime;
};

#endif /* _TICKER_H_ */
