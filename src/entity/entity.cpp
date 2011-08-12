#include "../config.h"
#include "entity.h"

Entity::Entity(ImageManager *imageManager) {
  sprite = new sf::Sprite(*(imageManager->get("cube")));
  offset = sf::Vector2f(0, 0);
  position[0] = 0;
  position[1] = 0;
  bbox = sprite->GetSize();
  node = new ZCom_Node();  
  remove = false;
}

Entity::~Entity() {
  if(node)
    delete node;
  delete sprite;
}

void Entity::Draw(sf::RenderTarget *rt) {
  sprite->SetPosition(GetPosition());
  rt->Draw(*sprite);
}

void Entity::Update(float frametime){
}

sf::FloatRect Entity::GetBbox() {
  return sf::FloatRect( position[0], position[1], bbox.x, bbox.y);
}

sf::Vector2f Entity::GetPosition() {
  return sf::Vector2f(position[0], position[1]);
}

sf::Vector2f Entity::GetCenter() {
  sf::Vector2f hbbox = sf::Vector2f(bbox.x / 2, bbox.y / 2);
  return GetPosition() + hbbox;
}

void Entity::SetPosition(sf::Vector2f position){
  this->position[0] = position.x;
  this->position[1] = position.y;
}

bool Entity::CanRemove(){
  return remove;
}

void Entity::ProcessNodeEvents(){
  while(node->checkEventWaiting()) {
    eZCom_Event type;
    eZCom_NodeRole remoteRole;
    ZCom_ConnID connId;
    
    ZCom_BitStream *data = node->getNextEvent(&type, &remoteRole, &connId);
    
    if(remoteRole == eZCom_RoleAuthority && type == eZCom_EventRemoved)
      remove = true;  
 } 
}
