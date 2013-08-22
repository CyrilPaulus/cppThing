#ifndef _IMAGEMANAGER_H_
#define _IMAGEMANAGER_H_

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ImageManager {
 public:
  ~ImageManager();
  const sf::Texture *get(std::string);
  
 private:
  std::map<std::string, sf::Texture*> _image_set;
};

#endif /* _IMAGEMANAGER_H_ */
