#ifndef _WORLD_H_
#define _WORLD_H_

#include <SFML/Graphics.hpp>
#include <list>

#include "cube.h"
#include "../utils/quadtree.h"

class World {
 public:
  World(ZCom_Control*, ImageManager *, bool);
  ~World();
  
  void Draw(sf::RenderTarget *);
  void Update();
  void AddCube(sf::Vector2f, int);
  void RemoveCube(sf::Vector2f);

  Cube* GetCollidingCube(sf::FloatRect);
 private:
  ImageManager* imageManager;
  ZCom_Control* control;
  bool server;
  std::list<Cube*> cubeList;
  QuadTree *quadTree;
};

#endif /* _WORLD_H_ */
