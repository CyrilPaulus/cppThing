#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SFML/Graphics.hpp>
#include "imageManager.h"

class Screen {
	public:
	
	protected:
		Screen(sf::RenderWindow *window, ImageManager *imageManager);
		~Screen();
		int Run();
		
		ImageManager *imageManager;
		sf::RenderWindow *window;
		
	private:
		
};

#endif /* _SCREEN_H_ */