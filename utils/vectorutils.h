#ifndef _VECTORUTILS_H_
#define _VECTORUTILS_H_

#include <SFML/Graphics.hpp>

class VectorUtils{
 public:
  static float distance(sf::Vector2f, sf::Vector2f);
  static sf::Vector2f normalize(sf::Vector2f);
};


#endif /* _VECTORUTILS_H_ */
