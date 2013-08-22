#include "../config.h"
#include "world.h"
#include "math.h"

#include <iostream>
#include <fstream>

using namespace std;

World::World(bool server){
  for(int i = 0; i < GameConstant::LAYERNBR; i++){
    this->quadTrees[i] = new QuadTree(10, sf::Vector2f(90, 90));
  }  
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

void World::addCube(sf::Vector2f pos, int type, int layerIndex, bool force){
  sf::Vector2f gridPos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);  
  
  if(force || canAddCube(pos, layerIndex)) {
    Cube* cube = new Cube(type);
    cube->setPosition(gridPos);
    layer[layerIndex].push_back(cube);
    quadTrees[layerIndex]->Add(cube);
  } 
}

bool World::canAddCube(sf::Vector2f pos, int layerIndex) {
  sf::Vector2f gridPos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);
  sf::FloatRect bbox(gridPos.x, gridPos.y, Cube::WIDTH, Cube::HEIGHT);
  
  std::list<Cube*>::iterator i;
  std::list<Cube*> chunk = quadTrees[layerIndex]->GetList(bbox);
  for(i = chunk.begin(); i != chunk.end(); i++) {
    if((*i)->getBbox().intersects(bbox)) 
      return false;      
  }
  
  if(layerIndex == 1) {
    std::list<Player*>::iterator p;
    for(p = playerList.begin(); p != playerList.end(); p++)
      if((*p)->getBbox().intersects(bbox))
	return false;
  }
  
  return true;
}

bool World::canRemoveCube(sf::Vector2f pos, int layerIndex) {
  sf::Vector2f gridPos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);
  sf::FloatRect bbox(gridPos.x, gridPos.y, Cube::WIDTH, Cube::HEIGHT);
  
  std::list<Cube*>::iterator i;
    std::list<Cube*> chunk = quadTrees[layerIndex]->GetList(bbox);
    for(i = chunk.begin(); i != chunk.end(); i++) {
      if((*i)->getBbox().intersects(bbox)) 
	return true;      
    }
    
    return false;
}

void World::addCube(sf::Vector2f pos, int type, int layerIndex) {
  addCube(pos, type, layerIndex, false);
}

void World::removeCube(sf::Vector2f pos, int layerIndex) {
  std::list<Cube*>::iterator c;
  for( c = layer[layerIndex].begin(); c != layer[layerIndex].end(); c++) {
    if((*c)->getBbox().contains(pos.x, pos.y)) {
      quadTrees[layerIndex]->Remove((*c));
      delete (*c);
      c = layer[layerIndex].erase(c);      
    }
  }
}

void World::addPlayer(Player *p) {
  playerList.push_back(p);
}

void World::removePlayer(Player *p) {
  playerList.remove(p);
  delete(p);
}

Cube* World::getCollidingCube(sf::FloatRect bbox){
  std::list<Cube*> candidate = quadTrees[1]->GetList(bbox);
  std::list<Cube*>::iterator it;
  for(it = candidate.begin(); it != candidate.end(); it++){
    if((*it)->getBbox().intersects(bbox))
      return (*it);
  }
  return NULL;
} 

void World::update() {
  //TODO find a better way to delete
  std::list<Cube*>::iterator c;  
  for(int i = 0; i < GameConstant::LAYERNBR; i++) {
    for(c = layer[i].begin(); c != layer[i].end(); c++) {
      if((*c)->canRemove()){
	quadTrees[i]->Remove((*c));
	delete (*c);
	c = layer[i].erase(c);
      }	
    }
  }
   
  std::list<Player*> toDeleteP;
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++) {
    if((*p)->canRemove())
      toDeleteP.push_back(*p);
  }

  for(p = toDeleteP.begin(); p != toDeleteP.end(); p++) {
    removePlayer((*p));
  }
}

void World::updatePlayer(sf::Time frametime, Input input) {
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++)
    (*p)->update(frametime, input);
}

std::list<Cube*> World::getList(int i) {
  return this->layer[i];
}

void World::removePlayerById(int id) {
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++) {
    if((*p)->getId() == id){
      playerList.erase(p);
      return;
    } 
  }
}

Player* World::getPlayerById(int id) {
  std::list<Player*>::iterator p;
  for(p = playerList.begin(); p != playerList.end(); p++) {
    if((*p)->getId() == id){
      return(*p);
    } 
  }
  return NULL;
}

std::list<Player*> World::getPlayerList() {
    return playerList;
}

bool World::save(std::string filename) {
  ofstream file(filename.c_str(), ios::out | ios::binary);
  
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return false;
  }
    
  int layer_nbr = GameConstant::LAYERNBR;
  file.write((char*)&layer_nbr, sizeof(int));
  for(int i = 0; i < GameConstant::LAYERNBR; i++) {
    int size = layer[i].size();
    file.write((char*)&size, sizeof(int));
    std::list<Cube*>::iterator b;
    for(b = layer[i].begin(); b != layer[i].end(); b++) {
      int type = (*b)->getType();
      float x = (*b)->getPosition().x;
      float y = (*b)->getPosition().y;
      file.write((char*)&type, sizeof(int));
      file.write((char*)&x, sizeof(float));
      file.write((char*)&y, sizeof(float));
    }
  }  
 
  file.close();
  return true;
}

bool World::load(std::string filename) {
  ifstream file(filename.c_str(), ios::in | ios::binary);
  
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return false;
  }
 
  int nbr_layers = 0;;
  file.read((char*)&nbr_layers, sizeof(int));
  for(int i = 0; i < nbr_layers; i++) {
    int nbr_cubes = 0;
    file.read((char*)&nbr_cubes, sizeof(int));
    for(int j = 0; j < nbr_cubes; j++) {
      int type = 0;
      float x = 0;
      float y = 0;
      file.read((char*)&type, sizeof(int));
      file.read((char*)&x, sizeof(float));
      file.read((char*)&y, sizeof(float));
      addCube(sf::Vector2f(x, y), type, i, true);
    }
  }
  
  file.close();
  return true;
}
