#include "screen.h"
#include "config.h"

Screen::Screen(sf::RenderWindow *window, ImageManager* imageManager) {
	this->window = window;
	this->imageManager = imageManager;
}

Screen::~Screen() {
}