#ifndef _LAYERDISPLAY_H_
#define _LAYERDISPLAY_H_

#include "uiElement.h"

class LayerDisplay : public UiElement {
 public:
  LayerDisplay(ImageManager*, int);
  ~LayerDisplay();
  void SetLayer(int);
  void Draw(sf::RenderTarget*);
  sf::FloatRect GetBbox();
  sf::Vector2f GetSize();
  
 private:
  sf::Sprite* _layer;
  int _layer_count;
  int _current_layer;
};

#endif /* _LAYERDISPLAY_H_ */
