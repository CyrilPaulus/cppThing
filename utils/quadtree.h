#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <SFML/Graphics.hpp>
#include <list>
#include "../entity/cube.h"

class QuadTreeNode {
 public:
  QuadTreeNode(sf::FloatRect, int, sf::Vector2f);
  ~QuadTreeNode();
  void add(Cube*);
  void explode();
  void remove(Cube*);
  std::list<Cube*> getList(sf::FloatRect);

 private:
  std::list<QuadTreeNode*> _children;
  std::list<Cube*> _cube_list;
  int _capacity;
  sf::FloatRect _range;
  sf::Vector2f _min_size;
};

class QuadTree {
 public:
  QuadTree(int, sf::Vector2f);
  ~QuadTree();
  void add(Cube *);
  void remove(Cube *);
  std::list<Cube*> getList(sf::FloatRect bbox);

 private:
  int _capacity;
  sf::Vector2f _min_size;
  QuadTreeNode* _root;
};

#endif /* _QUADTREE_H_ */
