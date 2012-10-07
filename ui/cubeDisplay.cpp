#include "cubeDisplay.h"
#include "../config.h"


CubeDisplay::CubeDisplay(ImageManager* imageManager) : UiElement(imageManager) {
  this->cube = new Cube(0);
  this->r = new Renderer(imageManager);
}

CubeDisplay::~CubeDisplay() {
	delete cube;
        delete r;
}

void CubeDisplay::SetType(int type) {
	cube->setType(type);
}

void CubeDisplay::SetPosition(sf::Vector2f pos) {
	UiElement::SetPosition(pos);
	cube->setPosition(pos);
}

void CubeDisplay::Draw(sf::RenderTarget *rt) {
        r->renderCube(cube, rt);
}

sf::Vector2f CubeDisplay::GetSize() {
	return sf::Vector2f(Cube::WIDTH, Cube::HEIGHT);
}

sf::FloatRect CubeDisplay::GetBbox() {
	return sf::FloatRect(position.x, position.y, Cube::WIDTH, Cube::HEIGHT);
}