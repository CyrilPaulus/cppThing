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

  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++)
    delete (*p);
}

void World::Draw(sf::RenderTarget *rt){
  std::list<Cube*>::iterator i;
  for(i = cubeList.begin(); i != cubeList.end(); i++)
    (*i)->Draw(rt);

  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++)
    (*p)->Draw(rt);
}

void World::AddCube(sf::Vector2f pos, int type, bool force){
  sf::Vector2f gridPos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);
  sf::FloatRect bbox(gridPos.x, gridPos.y, Cube::WIDTH, Cube::HEIGHT);
  
  bool exist = false;
  
  if(!force) {
    std::list<Cube*>::iterator i;
    std::list<Cube*> chunk = quadTree->GetList(bbox);
    for(i = chunk.begin(); i != chunk.end(); i++) {
      if((*i)->GetBbox().Intersects(bbox)) {
	exist = true;
	break;
      }
    }
    
    if(!exist) {
      std::list<Player*>::iterator p;
      for(p = playerList.begin(); p != playerList.end(); p++)
	if((*p)->GetBbox().Intersects(bbox)) {
	  exist = true;
	  break;
	}
    }
  }
  
  if(!exist) {
    Cube* cube = new Cube(imageManager, type);
    cube->SetPosition(gridPos);
    cube->RegisterZCom(control, server);    
    cubeList.push_back(cube);
    quadTree->Add(cube);
  } 
}

void World::AddCube(sf::Vector2f pos, int type) {
  AddCube(pos, type, false);
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

void World::AddPlayer(Player *p) {
  playerList.push_back(p);
}

void World::RemovePlayer(Player *p) {
  playerList.remove(p);
  delete(p);
}

void World::RemovePlayerByID(ZCom_ConnID id){
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++) {
    if((*p)->GetID() == id) {
      delete(*p);
      playerList.erase(p);
      break;
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
  //TODO find a bette way to delete
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

  std::list<Player*> toDeleteP;
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++) {
    (*p)->ProcessNodeEvents();
    if((*p)->CanRemove())
      toDeleteP.push_back(*p);
  }

  for(p = toDeleteP.begin(); p != toDeleteP.end(); p++) {
    RemovePlayer((*p));
  }
}

Player *World::GetPlayerByID(ZCom_ConnID id){
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++){
    if((*p)->GetID() == id)
      return (*p);
  }
  return NULL;
} 
