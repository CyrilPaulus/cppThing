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
  World(bool server);
  ~World();
  
  void update();
  void updatePlayer(sf::Time frametime, Input);
  void addCube(sf::Vector2f, int, int);
  void addCube(sf::Vector2f, int, int, bool);
  void removeCube(sf::Vector2f, int);
  bool canAddCube(sf::Vector2f, int);
  bool canRemoveCube(sf::Vector2f, int);

  std::list<Cube*> getList(int layer);
  std::list<Player*> getPlayerList();
  void addPlayer(Player *);
  void removePlayer(Player *);
  void removePlayerById(int id);
  Player* getPlayerById(int id);
  Cube* getCollidingCube(sf::FloatRect);

  bool save(std::string filename);
  bool load(std::string filename);

 private:
  bool _server;
  std::list<Cube*> _layers[GameConstant::LAYERNBR];
  std::list<Player*> _player_list;
  QuadTree* _quad_trees[GameConstant::LAYERNBR];
};

#endif /* _WORLD_H_ */
