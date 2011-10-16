#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SFML/Graphics.hpp>
#include "imageManager.h"

class Screen {
	public:
		Screen(sf::RenderWindow *window, ImageManager *imageManager);
		virtual ~Screen();
		virtual int Run() =0;
		static const int NONE = -2;
		static const int EXIT = -1;
		static const int MAINMENU = 0;
		static const int GAME = 1;
		static const int CONNECT = 2;
		static const int OPTION = 3;
		static const int LOAD = 4;
		static const int SAVE = 5;
	
	protected:		
		ImageManager *imageManager;
		sf::RenderWindow *window;
		
	private:
		
};

#endif /* _SCREEN_H_ */
