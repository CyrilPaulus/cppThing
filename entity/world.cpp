#include "../config.h"
#include "world.h"
#include "math.h"

#include <iostream>
#include <fstream>

using namespace std;

World::World(bool server){
  for(int i = 0; i < GameConstant::LAYERNBR; i++){
    this->_quad_trees[i] = new QuadTree(10, sf::Vector2f(90, 90));
  }  
  _server = server;
}

World::~World() {
  for(int i = 0; i< GameConstant::LAYERNBR; i++) {
    delete _quad_trees[i];
  }
  
  for(int i = 0; i < GameConstant::LAYERNBR; i++) {
    std::list<Cube*>::iterator c;
    for(c = _layers[i].begin(); c != _layers[i].end(); c++)
      delete (*c);
  }
  
  std::list<Player*>::iterator p;
  for(p = _player_list.begin(); p != _player_list.end(); p++)
    delete (*p);
}

void World::addCube(sf::Vector2f pos, int type, int layer_index, bool force){
  sf::Vector2f grid_pos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);  
  
  if(force || canAddCube(pos, layer_index)) {
    Cube* cube = new Cube(type);
    cube->setPosition(grid_pos);
    _layers[layer_index].push_back(cube);
    _quad_trees[layer_index]->add(cube);
  } 
}

bool World::canAddCube(sf::Vector2f pos, int layer_index) {
  sf::Vector2f grid_pos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);
  sf::FloatRect bbox(grid_pos.x, grid_pos.y, Cube::WIDTH, Cube::HEIGHT);
  
  std::list<Cube*>::iterator i;
  std::list<Cube*> chunk = _quad_trees[layer_index]->getList(bbox);
  for(i = chunk.begin(); i != chunk.end(); i++) {
    if((*i)->getBbox().intersects(bbox)) 
      return false;      
  }
  
  if(layer_index == 1) {
    std::list<Player*>::iterator p;
    for(p = _player_list.begin(); p != _player_list.end(); p++)
      if((*p)->getBbox().intersects(bbox))
	return false;
  }
  
  return true;
}

bool World::canRemoveCube(sf::Vector2f pos, int layer_index) {
  sf::Vector2f grid_pos = sf::Vector2f(floor(pos.x / Cube::WIDTH) * Cube::WIDTH, floor(pos.y / Cube::HEIGHT) * Cube::HEIGHT);
  sf::FloatRect bbox(grid_pos.x, grid_pos.y, Cube::WIDTH, Cube::HEIGHT);
  
  std::list<Cube*>::iterator i;
    std::list<Cube*> chunk = _quad_trees[layer_index]->getList(bbox);
    for(i = chunk.begin(); i != chunk.end(); i++) {
      if((*i)->getBbox().intersects(bbox)) 
	return true;      
    }
    
    return false;
}

void World::addCube(sf::Vector2f pos, int type, int layer_index) {
  addCube(pos, type, layer_index, false);
}

void World::removeCube(sf::Vector2f pos, int layer_index) {
  std::list<Cube*>::iterator c;
  for( c = _layers[layer_index].begin(); c != _layers[layer_index].end(); c++) {
    if((*c)->getBbox().contains(pos.x, pos.y)) {
      _quad_trees[layer_index]->remove((*c));
      delete (*c);
      c = _layers[layer_index].erase(c);      
    }
  }
}

void World::addPlayer(Player *p) {
  _player_list.push_back(p);
}

void World::removePlayer(Player *p) {
  _player_list.remove(p);
  delete(p);
}

Cube* World::getCollidingCube(sf::FloatRect bbox){
  std::list<Cube*> candidate = _quad_trees[1]->getList(bbox);
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
    for(c = _layers[i].begin(); c != _layers[i].end(); c++) {
      if((*c)->canRemove()){
	_quad_trees[i]->remove((*c));
	delete (*c);
	c = _layers[i].erase(c);
      }	
    }
  }
   
  std::list<Player*> toDeleteP;
  std::list<Player*>::iterator p;
  for(p = _player_list.begin(); p != _player_list.end(); p++) {
    if((*p)->canRemove())
      toDeleteP.push_back(*p);
  }

  for(p = toDeleteP.begin(); p != toDeleteP.end(); p++) {
    removePlayer((*p));
  }
}

void World::updatePlayer(sf::Time frametime, Input input) {
  std::list<Player*>::iterator p;
  for(p = _player_list.begin(); p != _player_list.end(); p++)
    (*p)->update(frametime, input);
}

std::list<Cube*> World::getList(int i) {
  return this->_layers[i];
}

void World::removePlayerById(int id) {
  std::list<Player*>::iterator p;
  for(p = _player_list.begin(); p != _player_list.end(); p++) {
    if((*p)->getId() == id){
      _player_list.erase(p);
      return;
    } 
  }
}

Player* World::getPlayerById(int id) {
  std::list<Player*>::iterator p;
  for(p = _player_list.begin(); p != _player_list.end(); p++) {
    if((*p)->getId() == id){
      return(*p);
    } 
  }
  return NULL;
}

std::list<Player*> World::getPlayerList() {
    return _player_list;
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
    int size = _layers[i].size();
    file.write((char*)&size, sizeof(int));
    std::list<Cube*>::iterator b;
    for(b = _layers[i].begin(); b != _layers[i].end(); b++) {
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
