#include "../config.h"
#include "world.h"
#include "math.h"

World::World(ZCom_Control *control, ImageManager *imageManager, bool server){
  this->imageManager = imageManager;
  this->quadTree = new QuadTree(10, sf::Vector2f(90, 90));  
  this->control = control;
  this->server = server;
}

World::~World() {
  delete quadTree;
  std::list<Cube*>::iterator i;
  for(i = cubeList.begin(); i != cubeList.end(); i++)
    delete (*i);
}

void World::Draw(sf::RenderTarget *rt){
  std::list<Cube*>::iterator i;
  for(i = cubeList.begin(); i != cubeList.end(); i++)
    (*i)->Draw(rt);
}

void World::AddCube(sf::Vector2f pos, int type){
  sf::Vector2f gridPos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);
  sf::FloatRect bbox(gridPos.x, gridPos.y, Cube::WIDTH, Cube::HEIGHT);
  
  bool exist = false;
  
  std::list<Cube*>::iterator i;
  std::list<Cube*> chunk = quadTree->GetList(bbox);
  for(i = chunk.begin(); i != chunk.end(); i++) {
    if((*i)->GetBbox().Intersects(bbox)) {
      exist = true;
      break;
    }
  }
  
  if(!exist) {
    Cube* cube = new Cube(control, type, gridPos, imageManager, server);
    cubeList.push_back(cube);
    quadTree->Add(cube);
  } 
}

void World::RemoveCube(sf::Vector2f pos) {
  std::list<Cube*>::iterator i;
  for( i = cubeList.begin(); i != cubeList.end(); i++) {
    if((*i)->GetBbox().Contains(pos.x, pos.y)) {
      quadTree->Remove((*i));
      delete (*i);
      i = cubeList.erase(i);      
    }
  }
}

Cube* World::GetCollidingCube(sf::FloatRect bbox){
  std::list<Cube*> candidate = quadTree->GetList(bbox);
  std::list<Cube*>::iterator it;
  for(it = candidate.begin(); it != candidate.end(); it++){
    if((*it)->GetBbox().Intersects(bbox))
      return (*it);
  }
  return NULL;
} 

void World::Update() {
  std::list<Cube*> toDelete;
  std::list<Cube*>::iterator i;
  for(i = cubeList.begin(); i != cubeList.end(); i++) {
    (*i)->ProcessNodeEvents();
    if((*i)->CanRemove())
      toDelete.push_back(*i);
  }

  for(i = toDelete.begin(); i != toDelete.end(); i++) {
    RemoveCube((*i)->GetPosition());
  }
}

 
