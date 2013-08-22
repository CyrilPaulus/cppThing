#include "config.h"
#include "imageManager.h"

ImageManager::~ImageManager() {
  std::map<std::string, sf::Texture*>:: iterator i;
  for (i = _image_set.begin(); i != _image_set.end(); i++)
    delete i->second;
}

const sf::Texture *ImageManager::get(std::string name) {
  sf::Texture *value = _image_set[name];
  if(value != NULL)
    return value;

  value = new sf::Texture();
  if(!value->loadFromFile("res/" + name + ".png")) {
    std::cerr << "Can't load image: " + name << std::endl;
    return NULL;
  }

  _image_set[name] = value;
  return value;
}
