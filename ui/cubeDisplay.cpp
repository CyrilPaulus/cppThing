#include "cubeDisplay.h"
#include "../config.h"

CubeDisplay::CubeDisplay(ImageManager *image_manager) : UiElement(image_manager)
{
  _cube = new Cube(0);
  _r = new Renderer(image_manager);
}

CubeDisplay::~CubeDisplay()
{
  delete _cube;
  delete _r;
}

void CubeDisplay::SetType(int type)
{
  _cube->setType(type);
}

void CubeDisplay::SetPosition(sf::Vector2f pos)
{
  UiElement::SetPosition(pos);
  _cube->setPosition(pos);
}

void CubeDisplay::Draw(sf::RenderTarget *rt)
{
  _r->renderCube(_cube, rt);
}

sf::Vector2f CubeDisplay::GetSize()
{
  return sf::Vector2f(Cube::WIDTH, Cube::HEIGHT);
}

sf::FloatRect CubeDisplay::GetBbox()
{
  return sf::FloatRect(_position.x, _position.y, Cube::WIDTH, Cube::HEIGHT);
}
