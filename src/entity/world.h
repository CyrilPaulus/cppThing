#ifndef _WORLD_H_
#define _WORLD_H_

#include <SFML/Graphics.hpp>
#include <list>

#include "cube.h"
#include "../utils/quadtree.h"

class World {
 public:
  World(ImageManager *);
  ~World();
  
  void Draw(sf::RenderTarget *);
  
  void AddCube(sf::Vector2f, int);
  void RemoveCube(sf::Vector2f);

  Cube* GetCollidingCube(sf::FloatRect);
 private:
  ImageManager* imageManager;
  std::list<Cube*> cubeList;
  QuadTree *quadTree;
};

#endif /* _WORLD_H_ */
