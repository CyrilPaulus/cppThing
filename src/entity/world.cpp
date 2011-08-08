#include "../config.h"
#include "world.h"
#include "math.h"

World::World(ImageManager *imageManager){
  this->imageManager = imageManager;
  this->quadTree = new QuadTree(10, sf::Vector2f(90, 90));  
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
  Cube* cube = new Cube(type, imageManager);
  cube->SetPosition(gridPos);
  bool exist = false;
  
  std::list<Cube*>::iterator i;
  std::list<Cube*> chunk = quadTree->GetList(cube->GetBbox());
  for(i = chunk.begin(); i != chunk.end(); i++) {
    if((*i)->GetBbox().Intersects(cube->GetBbox())) {
      exist = true;
      break;
    }
  }
  
  if(!exist) {
    cubeList.push_back(cube);
    quadTree->Add(cube);
  }
  else
    delete cube;
}

void World::RemoveCube(sf::Vector2f pos) {
  std::list<Cube*>::iterator i;
  for( i = cubeList.begin(); i != cubeList.end(); i++) {
    if((*i)->GetBbox().Contains(pos.x, pos.y)) {
      quadTree->Remove((*i));
      i = cubeList.erase(i);
    }
  }
}

 


 
