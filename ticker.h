#ifndef _TICKER_H_
#define _TICKER_H_

#include <SFML/System/Clock.hpp>

class Ticker
{
public:
  Ticker();
  Ticker(sf::Time tickTime);
  bool tick();
  sf::Time getElapsedTime();
  void setUpdateRate(int);

private:
  sf::Clock _clock;
  sf::Time _tick_time;
  sf::Time _elapsed_time;
};

#endif /* _TICKER_H_ */
