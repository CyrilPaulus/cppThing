#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <SFML/Graphics.hpp>
#include <list>
#include "../entity/cube.h"

class QuadTreeNode {
 public:
  QuadTreeNode(sf::FloatRect, int, sf::Vector2f);
  ~QuadTreeNode();
  void Add(Cube*);
  void Explode();
  void Remove(Cube*);
  std::list<Cube*> GetList(sf::FloatRect);

 private:
  std::list<QuadTreeNode*> children;
  std::list<Cube*> cubeList;
  int capacity;
  sf::FloatRect range;
  sf::Vector2f minSize;
};

class QuadTree {
 public:
  QuadTree(int, sf::Vector2f);
  ~QuadTree();
  void Add(Cube *);
  void Remove(Cube *);
  std::list<Cube*> GetList(sf::FloatRect bbox);
 private:
  int capacity;
  sf::Vector2f minSize;
  QuadTreeNode *root;
};

#endif /* _QUADTREE_H_ */
