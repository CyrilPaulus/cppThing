#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SFML/Graphics.hpp>
#include "imageManager.h"

class Screen {
	public:
		Screen(sf::RenderWindow *window, ImageManager *imageManager);
		virtual ~Screen();
		virtual int Run() =0;
	
	protected:		
		ImageManager *imageManager;
		sf::RenderWindow *window;
		
	private:
		
};

#endif /* _SCREEN_H_ */