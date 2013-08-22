#include "../config.h"
#include "vectorutils.h"
#include <math.h>

float VectorUtils::distance(sf::Vector2f ori, sf::Vector2f dest) {
  sf::Vector2f diff = dest - ori;
  return (float)sqrt(diff.x * diff.x + diff.y * diff.y);
}

sf::Vector2f VectorUtils::normalize(sf::Vector2f ori){
  float dist = distance(sf::Vector2f(0, 0), ori);
  return sf::Vector2f(ori.x / dist, ori.y / dist);
}
