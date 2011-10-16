#include "layerDisplay.h"
#include "../config.h"

LayerDisplay::LayerDisplay(ImageManager* imageManager, int count) : UiElement(imageManager) {
  this->layer = new sf::Sprite();
  this->layer->SetTexture(*(imageManager)->get("layer"));
  this->layerCount = count;
  this->currentLayer = 0;
}

void LayerDisplay::SetLayer(int layer) {
  this->currentLayer = layer;
}

void LayerDisplay::Draw(sf::RenderTarget *rt) {
  layer->SetPosition(position);
  for(int i = 0; i < layerCount; i++) {
    if(i == currentLayer) {
      layer->SetPosition(layer->GetPosition() - sf::Vector2f(0, 10));
      rt->Draw(*layer);
      layer->SetPosition(layer->GetPosition() + sf::Vector2f(0, 10));
    }
    else
      rt->Draw(*layer);

    layer->SetPosition(layer->GetPosition() + sf::Vector2f(5, 5));
  }
}

LayerDisplay::~LayerDisplay() {
  delete layer;
}

sf::Vector2f LayerDisplay::GetSize() {
	float width = layerCount * 5 + layer->GetSize().x;
	float height = 10 + layerCount * 5 + layer->GetSize().y;
	return sf::Vector2f(width, height);
}

sf::FloatRect LayerDisplay::GetBbox() {
	return sf::FloatRect(position.x, position.y, GetSize().x, GetSize().y);
}