#include "config.h"
#include "imageManager.h"

ImageManager::~ImageManager() {
  std::map<std::string, sf::Texture*>:: iterator i;
  for (i = imageSet.begin(); i != imageSet.end(); i++)
    delete i->second;
}

sf::Texture *ImageManager::get(std::string name) {
  sf::Texture *value = imageSet[name];
  if(value != NULL)
    return value;

  value = new sf::Texture();
  if(!value->LoadFromFile("res/" + name + ".png")) {
    std::cerr << "Can't load image: " + name << std::endl;
    return NULL;
  }

  imageSet[name] = value;
  return value;
}
