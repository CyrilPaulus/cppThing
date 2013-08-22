#ifndef _CUBEDISPLAY_H_
#define _CUBEDISPLAY_H_

#include "uiElement.h"
#include "../entity/cube.h"
#include "../Renderer.h"

class CubeDisplay : public UiElement {
 public:
  CubeDisplay(ImageManager*);
  ~CubeDisplay();
  void SetType(int);
  void Draw(sf::RenderTarget*);
  void SetPosition(sf::Vector2f pos);
  sf::FloatRect GetBbox();
  sf::Vector2f GetSize();
  
 private:
  Cube* _cube;
  //Todo remove
  Renderer* _r;
};

#endif /* _CUBEDISPLAY_H_ */
