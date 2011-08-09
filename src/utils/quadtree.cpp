#include "../config.h"
#include "quadtree.h"

QuadTree::QuadTree(int capacity, sf::Vector2f minSize) {
  this->capacity = capacity;
  this->minSize = minSize;
  root = new QuadTreeNode(sf::FloatRect(-1000000, -1000000, 2000000, 2000000), capacity, minSize);
}

QuadTree::~QuadTree() {
  delete root;
}

void QuadTree::Add(Cube *b) {
  root->Add(b);
}

void QuadTree::Remove(Cube *b) {
  root->Remove(b);
}

std::list<Cube*> QuadTree::GetList(sf::FloatRect bbox) {
  return root->GetList(bbox);
}

QuadTreeNode::QuadTreeNode(sf::FloatRect range, int capacity, sf::Vector2f minSize) {
  this->range = range;
  this->capacity = capacity;
  this->minSize = minSize;
}

QuadTreeNode::~QuadTreeNode() {
  if(!children.empty()) {
    std::list<QuadTreeNode*>::iterator i;
    for (i = children.begin(); i != children.end(); i++)
      delete (*i);
  }
}

void QuadTreeNode::Add(Cube *b) {
  if(!children.empty()){
    std::list<QuadTreeNode*>::iterator i;
    for (i = children.begin(); i != children.end(); i++)
      if ((*i)->range.Intersects(b->GetBbox()))
	(*i)->Add(b);	  
  } else if ((int)cubeList.size() < capacity || (range.Width <= minSize.x || range.Height <= minSize.y)) {
    cubeList.push_back(b);
  } else {
    Explode();
    Add(b);
  }
} 

void QuadTreeNode::Explode() {
  if(range.Width <= minSize.x || range.Height <= minSize.y)
    return;

  float width = range.Width / 2;
  float height = range.Height / 2;
  children.push_back(new QuadTreeNode(sf::FloatRect(range.Left, range.Top, width, height), capacity, minSize));
  children.push_back(new QuadTreeNode(sf::FloatRect(range.Left + width, range.Top, width, height), capacity, minSize));
  children.push_back(new QuadTreeNode(sf::FloatRect(range.Left, range.Top + height, width, height), capacity, minSize));
  children.push_back(new QuadTreeNode(sf::FloatRect(range.Left + width, range.Top + height, width, height), capacity, minSize));

  std::list<Cube*>::iterator i;
  for (i = cubeList.begin(); i != cubeList.end(); i++){
    Add(*i);
  }
  cubeList.clear();
}

void QuadTreeNode::Remove(Cube *b) {
  if(!children.empty()) {
    std::list<QuadTreeNode*>::iterator i;
    for (i = children.begin(); i != children.end(); i++)
      if((*i)->range.Intersects(b->GetBbox()))
	(*i)->Remove(b);
  }
  else
    cubeList.remove(b);
}

std::list<Cube*> QuadTreeNode::GetList(sf::FloatRect bbox) {
  if(!children.empty()) {
    std::list<Cube*> rtn;
    std::list<QuadTreeNode*>::iterator i;
    for (i = children.begin(); i != children.end(); i++) {
      if((*i)->range.Intersects(bbox)) {
	std::list<Cube*> candidate = (*i)->GetList(bbox);
	std::list<Cube*>::iterator c;
	for (c = candidate.begin(); c != candidate.end(); c++)
	  rtn.push_back((*c));
      }
    }
    return rtn;
  }
  else 
    return cubeList;
}



