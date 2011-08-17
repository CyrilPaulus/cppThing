#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

class GameConstant{
 public:
  static const int FRAMERATE_LIMIT = 60;
  static const unsigned int UPDATE_RATE = 50;
  static const unsigned int SIMULATION_TIME_PER_UPDATE = 1000 / UPDATE_RATE;
  static const int LAYERNBR = 3;
  static const sf::Color BackgroundColor;
};

#endif
