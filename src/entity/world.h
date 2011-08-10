#ifndef _WORLD_H_
#define _WORLD_H_

#include <SFML/Graphics.hpp>
#include <list>

#include "cube.h"
#include "../utils/quadtree.h"
#include "player.h"

class Player;

class World {
 public:
  World(ZCom_Control*, ImageManager *, bool);
  ~World();
  
  void Draw(sf::RenderTarget *);
  void Update();
  void AddCube(sf::Vector2f, int);
  void AddCube(sf::Vector2f, int, bool);
  void RemoveCube(sf::Vector2f);

  void AddPlayer(Player *);
  void RemovePlayer(Player *);
  void RemovePlayerByID(ZCom_ConnID);
  Cube* GetCollidingCube(sf::FloatRect);
 private:
  ImageManager* imageManager;
  ZCom_Control* control;
  bool server;
  std::list<Cube*> cubeList;
  std::list<Player*> playerList;
  QuadTree *quadTree;
};

#endif /* _WORLD_H_ */
