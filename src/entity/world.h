#ifndef _WORLD_H_
#define _WORLD_H_

#include <SFML/Graphics.hpp>
#include <list>

#include "cube.h"
#include "../utils/quadtree.h"
#include "player.h"
#include "../input/input.h"
#include "../config.h"

class Player;

class World {
 public:
  World(ImageManager *, bool);
  ~World();
  
  void draw(sf::RenderTarget *);
  void update();
  void updatePlayer(sf::Time frametime, Input);
  void addCube(sf::Vector2f, int, int);
  void addCube(sf::Vector2f, int, int, bool);
  void removeCube(sf::Vector2f, int);
  bool canAddCube(sf::Vector2f, int);
  bool canRemoveCube(sf::Vector2f, int);

  std::list<Cube*> getList(int layer);
  void addPlayer(Player *);
  void removePlayer(Player *);
  void removePlayerById(int id);
  Player* getPlayerById(int id);
  Cube* getCollidingCube(sf::FloatRect);
 private:
  ImageManager* imageManager;
  bool server;
  std::list<Cube*> layer[GameConstant::LAYERNBR];
  std::list<Player*> playerList;
  QuadTree* quadTrees[GameConstant::LAYERNBR];
};

#endif /* _WORLD_H_ */
