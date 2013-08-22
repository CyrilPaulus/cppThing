#include "../config.h"
#include "quadtree.h"

QuadTree::QuadTree(int capacity, sf::Vector2f min_size) {
  _capacity = capacity;
  _min_size = min_size;
  _root = new QuadTreeNode(sf::FloatRect(-1000000, -1000000, 2000000, 2000000), capacity, min_size);
}

QuadTree::~QuadTree() {
  delete _root;
}

void QuadTree::add(Cube *b) {
  _root->add(b);
}

void QuadTree::remove(Cube *b) {
  _root->remove(b);
}

std::list<Cube*> QuadTree::getList(sf::FloatRect bbox) {
  return _root->getList(bbox);
}

QuadTreeNode::QuadTreeNode(sf::FloatRect range, int capacity, sf::Vector2f min_size) {
  _range = range;
  _capacity = capacity;
  _min_size = min_size;
}

QuadTreeNode::~QuadTreeNode() {
  if(!_children.empty()) {
    std::list<QuadTreeNode*>::iterator i;
    for (i = _children.begin(); i != _children.end(); i++)
      delete (*i);
  }
}

void QuadTreeNode::add(Cube *b) {
  if(!_children.empty()){
    std::list<QuadTreeNode*>::iterator i;
    for (i = _children.begin(); i != _children.end(); i++)
      if ((*i)->_range.intersects(b->getBbox()))
	(*i)->add(b);	  
  } else if ((int)_cube_list.size() < _capacity || (_range.width <= _min_size.x || _range.height <= _min_size.y)) {
    _cube_list.push_back(b);
  } else {
    explode();
    add(b);
  }
} 

void QuadTreeNode::explode() {
  if(_range.width <= _min_size.x || _range.height <= _min_size.y)
    return;

  float width = _range.width / 2;
  float height = _range.height / 2;
  _children.push_back(new QuadTreeNode(sf::FloatRect(_range.left, _range.top, width, height), _capacity, _min_size));
  _children.push_back(new QuadTreeNode(sf::FloatRect(_range.left + width, _range.top, width, height), _capacity, _min_size));
  _children.push_back(new QuadTreeNode(sf::FloatRect(_range.left, _range.top + height, width, height), _capacity, _min_size));
  _children.push_back(new QuadTreeNode(sf::FloatRect(_range.left + width, _range.top + height, width, height), _capacity, _min_size));

  std::list<Cube*>::iterator i;
  for (i = _cube_list.begin(); i != _cube_list.end(); i++){
    add(*i);
  }
  _cube_list.clear();
}

void QuadTreeNode::remove(Cube *b) {
  if(!_children.empty()) {
    std::list<QuadTreeNode*>::iterator i;
    for (i = _children.begin(); i != _children.end(); i++)
      if((*i)->_range.intersects(b->getBbox()))
	(*i)->remove(b);
  }
  else
    _cube_list.remove(b);
}

std::list<Cube*> QuadTreeNode::getList(sf::FloatRect bbox) {
  if(!_children.empty()) {
    std::list<Cube*> rtn;
    std::list<QuadTreeNode*>::iterator i;
    for (i = _children.begin(); i != _children.end(); i++) {
      if((*i)->_range.intersects(bbox)) {
	std::list<Cube*> candidate = (*i)->getList(bbox);
	std::list<Cube*>::iterator c;
	for (c = candidate.begin(); c != candidate.end(); c++)
	  rtn.push_back((*c));
      }
    }
    return rtn;
  }
  else 
    return _cube_list;
}




