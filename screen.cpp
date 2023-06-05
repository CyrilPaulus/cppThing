#include "screen.h"
#include "config.h"

Screen::Screen(sf::RenderWindow *window, ImageManager *image_manager)
{
  _window = window;
  _image_manager = image_manager;
}

Screen::~Screen()
{
}
