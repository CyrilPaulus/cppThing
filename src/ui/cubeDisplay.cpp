#include "cubeDisplay.h"
#include "../config.h"

CubeDisplay::CubeDisplay(ImageManager* imageManager) : UiElement(imageManager) {
  this->cube = new Cube(imageManager, 0);
}

CubeDisplay::~CubeDisplay() {
	delete cube;
}

void CubeDisplay::SetType(int type) {
	cube->SetType(type);
}

void CubeDisplay::SetPosition(sf::Vector2f pos) {
	UiElement::SetPosition(pos);
	cube->SetPosition(pos);
}

void CubeDisplay::Draw(sf::RenderTarget *rt) {
	cube->Draw(rt);
}

sf::Vector2f CubeDisplay::GetSize() {
	return sf::Vector2f(Cube::WIDTH, Cube::HEIGHT);
}

sf::FloatRect CubeDisplay::GetBbox() {
	return sf::FloatRect(position.x, position.y, Cube::WIDTH, Cube::HEIGHT);
}