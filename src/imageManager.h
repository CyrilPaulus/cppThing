#ifndef _IMAGEMANAGER_H_
#define _IMAGEMANAGER_H_

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ImageManager {
 public:
  ~ImageManager();
  sf::Image *get(std::string);

 private:
  std::map<std::string, sf::Image*> imageSet;
};

#endif /* _IMAGEMANAGER_H_ */
