#ifndef _LAYERDISPLAY_H_
#define _LAYERDISPLAY_H_

#include "uiElement.h"

class LayerDisplay : public UiElement {
 public:
  LayerDisplay(ImageManager*, int);
  ~LayerDisplay();
  void SetLayer(int);
  void Draw(sf::RenderTarget*);
 private:
  sf::Sprite *layer;
  int layerCount;
  int currentLayer;
};

#endif /* _LAYERDISPLAY_H_ */
