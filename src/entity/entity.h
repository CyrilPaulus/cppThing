#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SFML/Graphics.hpp>
#include "../imageManager.h"
#include <zoidcom.h>
class Entity {
 public:
  Entity(ZCom_Control*, ImageManager *, bool server);
  ~Entity();

  void Draw(sf::RenderTarget *);
  void Update(float);
  sf::FloatRect GetBbox();
  sf::Vector2f GetPosition();
  void SetPosition(sf::Vector2f);
  sf::Vector2f GetCenter();
  
  static void RegisterClass(ZCom_Control * control, bool server);
  static ZCom_ClassID GetClass(bool server);

  void ProcessNodeEvents();
  bool CanRemove();

 protected:
  ZCom_Node* node;
  bool remove;
  
  static ZCom_ClassID netClassServerId;
  static ZCom_ClassID netClassClientId;
 
  sf::Sprite *sprite;
  sf::Vector2f position;
  sf::Vector2f offset;
  sf::Vector2f bbox;

};

#endif /* _ENTITY_H_ */
