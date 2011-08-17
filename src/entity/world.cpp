#include "../config.h"
#include "world.h"
#include "math.h"

World::World(ZCom_Control *control, ImageManager *imageManager, bool server){
  this->imageManager = imageManager;
  for(int i = 0; i < GameConstant::LAYERNBR; i++){
    this->quadTrees[i] = new QuadTree(10, sf::Vector2f(90, 90));
  }  
  this->control = control;
  this->server = server;
}

World::~World() {
  for(int i = 0; i< GameConstant::LAYERNBR; i++) {
    delete quadTrees[i];
  }

  for(int i = 0; i < GameConstant::LAYERNBR; i++) {
    std::list<Cube*>::iterator c;
    for(c = layer[i].begin(); c != layer[i].end(); c++)
      delete (*c);
  }

  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++)
    delete (*p);
}

void World::Draw(sf::RenderTarget *rt){
  for(int i = 0; i < GameConstant::LAYERNBR; i++) {
    std::list<Cube*>::iterator c;
    for(c = layer[i].begin(); c != layer[i].end(); c++)
      (*c)->Draw(rt);
  
    if(i == 0) {
      sf::Vector2f origin = rt->ConvertCoords(0,0);
      sf::Shape fog = sf::Shape::Rectangle(sf::FloatRect(origin.x, origin.y, rt->GetDefaultView().GetSize().x, 
			      rt->GetDefaultView().GetSize().y), sf::Color(100, 149, 237, 150));
      rt->Draw(fog);
    }
    if(i == 1){
      std::list<Player*>::iterator p;
      for(p = playerList.begin(); p != playerList.end(); p++)
	(*p)->Draw(rt);
    }
  }

 
}

void World::AddCube(sf::Vector2f pos, int type, int layerIndex, bool force){
  sf::Vector2f gridPos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);  
  
  if(force || CanAddCube(pos, layerIndex)) {
    Cube* cube = new Cube(imageManager, type);
    cube->SetPosition(gridPos);
    cube->RegisterZCom(control, server, layerIndex);    
    layer[layerIndex].push_back(cube);
    quadTrees[layerIndex]->Add(cube);
  } 
}

bool World::CanAddCube(sf::Vector2f pos, int layerIndex) {
  sf::Vector2f gridPos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);
  sf::FloatRect bbox(gridPos.x, gridPos.y, Cube::WIDTH, Cube::HEIGHT);
  
  std::list<Cube*>::iterator i;
  std::list<Cube*> chunk = quadTrees[layerIndex]->GetList(bbox);
  for(i = chunk.begin(); i != chunk.end(); i++) {
    if((*i)->GetBbox().Intersects(bbox)) 
      return false;      
  }
  
  if(layerIndex == 1) {
    std::list<Player*>::iterator p;
    for(p = playerList.begin(); p != playerList.end(); p++)
      if((*p)->GetBbox().Intersects(bbox))
	return false;
  }
  
  return true;
}

bool World::CanRemoveCube(sf::Vector2f pos, int layerIndex) {
  sf::Vector2f gridPos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);
  sf::FloatRect bbox(gridPos.x, gridPos.y, Cube::WIDTH, Cube::HEIGHT);
  
  std::list<Cube*>::iterator i;
    std::list<Cube*> chunk = quadTrees[layerIndex]->GetList(bbox);
    for(i = chunk.begin(); i != chunk.end(); i++) {
      if((*i)->GetBbox().Intersects(bbox)) 
	return true;      
    }
    
    return false;
}

void World::AddCube(sf::Vector2f pos, int type, int layerIndex) {
  AddCube(pos, type, layerIndex, false);
}

void World::RemoveCube(sf::Vector2f pos, int layerIndex) {
  std::list<Cube*>::iterator c;
  for( c = layer[layerIndex].begin(); c != layer[layerIndex].end(); c++) {
    if((*c)->GetBbox().Contains(pos.x, pos.y)) {
      quadTrees[layerIndex]->Remove((*c));
      delete (*c);
      c = layer[layerIndex].erase(c);      
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
  std::list<Cube*> candidate = quadTrees[1]->GetList(bbox);
  std::list<Cube*>::iterator it;
  for(it = candidate.begin(); it != candidate.end(); it++){
    if((*it)->GetBbox().Intersects(bbox))
      return (*it);
  }
  return NULL;
} 

void World::Update() {
  //TODO find a better way to delete
  std::list<Cube*>::iterator c;  
  for(int i = 0; i < GameConstant::LAYERNBR; i++) {
    for(c = layer[i].begin(); c != layer[i].end(); c++) {
      (*c)->ProcessNodeEvents();
      if((*c)->CanRemove()){
	quadTrees[i]->Remove((*c));
	delete (*c);
	c = layer[i].erase(c);
      }	
    }
  }
   
  std::list<Player*> toDeleteP;
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++) {
    if((*p)->CanRemove())
      toDeleteP.push_back(*p);
  }

  for(p = toDeleteP.begin(); p != toDeleteP.end(); p++) {
    RemovePlayer((*p));
  }
}

void World::UpdatePlayer(unsigned int frametime, Input input) {
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++)
    (*p)->Update(frametime, input);
}

Player *World::GetPlayerByID(ZCom_ConnID id){
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++){
    if((*p)->GetID() == id)
      return (*p);
  }
  return NULL;
} 
