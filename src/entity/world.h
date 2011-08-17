#ifndef _WORLD_H_
#define _WORLD_H_

#include <SFML/Graphics.hpp>
#include <list>

#include "cube.h"
#include "../utils/quadtree.h"
#include "player.h"
#include "../input/input.h"

class Player;

class World {
 public:
  World(ZCom_Control*, ImageManager *, bool);
  ~World();
  
  void Draw(sf::RenderTarget *);
  void Update();
  void UpdatePlayer(unsigned int, Input);
  void AddCube(sf::Vector2f, int, int);
  void AddCube(sf::Vector2f, int, int, bool);
  void RemoveCube(sf::Vector2f, int);
  bool CanAddCube(sf::Vector2f, int);
  bool CanRemoveCube(sf::Vector2f, int);

  void AddPlayer(Player *);
  void RemovePlayer(Player *);
  void RemovePlayerByID(ZCom_ConnID);
  Player* GetPlayerByID(ZCom_ConnID);
  Cube* GetCollidingCube(sf::FloatRect);
 private:
  ImageManager* imageManager;
  ZCom_Control* control;
  bool server;
  std::list<Cube*> layer[GameConstant::LAYERNBR];
  std::list<Player*> playerList;
  QuadTree* quadTrees[GameConstant::LAYERNBR];
};

#endif /* _WORLD_H_ */
