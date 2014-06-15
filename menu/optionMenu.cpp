#include "optionMenu.h"
#include "../config.h"

#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cctype>

//TODO: UBER DIRTY
#define ACTION_SAVE 15

std::string colToHex(sf::Vector3i color) {
  std::stringstream ss;
  ss << "#";
  ss << std::setfill ('0') << std::setw(2); 
  ss << std::hex << color.x;
  ss << std::hex << color.y;
  ss << std::hex << color.z;

  std::string str = ss.str();
  std::transform(str.begin(), str.end(),str.begin(), (int (*)(int))std::toupper);
  return str;
}

sf::Vector3i hexToCol(std::string hex) {
  if(hex.size() == 7) {
    int x;
    int y;
    int z;
    std::stringstream ssx;
    ssx << std::hex << hex.substr(1,2);
    ssx >> x;
    std::stringstream ssy;
    ssy << std::hex << hex.substr(3,2);
    ssy >> y;
    std::stringstream ssz;
    ssz << std::hex << hex.substr(5,2);
    ssz >> z;
    return sf::Vector3i(x, y, z);
  }
  

  return sf::Vector3i(-1,-1,-1);
}

int OptionMenu_Save() {
  return ACTION_SAVE;
}

int OptionMenu_Cancel() {
  return Screen::MAINMENU;
}

OptionMenu::OptionMenu(sf::RenderWindow* w, ImageManager* img, Game* game) : Screen(w, img) {
 
    r = new Renderer(img);
    mouse = new Mouse(w, w, img);

    _name = new MenuTextInput("Name", sf::Vector2f(0, 130), NULL);
    _color = new MenuTextInput("Color", sf::Vector2f(0, 160), NULL);
    
    items.push_back(_name);
    items.push_back(_color);

    items.push_back(new MenuItem("Save", sf::Vector2f(0, 240), &OptionMenu_Save));
    items.push_back(new MenuItem("Cancel", sf::Vector2f(0, 270), &OptionMenu_Cancel));

    selectedItem = 0;

    for (unsigned int i = 0; i < items.size(); i++)
        items[i]->CenterX(_window->getSize().x);
    running = true;
    this->game = game;
}

OptionMenu::~OptionMenu() {
  for(unsigned int i = 0; i < items.size(); i++)
    delete items[i];
  delete mouse;
  delete r;
}

int OptionMenu::run() {
  _name->SetText(game->getPseudo());
  _color->SetText(colToHex(game->getColor()));
  
  resize(_window->getSize().x, _window->getSize().y);

  sf::Event event;
  while (running) {
    while (_window->pollEvent(event)) {
      int rtn = handleEvent(event);
      if (rtn != Screen::NONE)
	return rtn;
    }
    
    mouse->update();
    
    _window->clear(sf::Color(100, 149, 237));
    
    for (unsigned int i = 0; i < items.size(); i++) {
      items[i]->Draw(_window);
      items[i]->SetActive(i == selectedItem);
      items[i]->CenterX(_window->getSize().x);
    }
    mouse->draw(_window);

    //color 
    sf::Vector3i col = hexToCol(_color->GetText());

    sf::RectangleShape shape(sf::Vector2f(50,50));
    if(col != sf::Vector3i(-1, -1, -1)) {
      shape.setPosition(_window->getSize().x /2 - 25, 195);
      shape.setFillColor(sf::Color(col.x, col.y, col.z));
      _window->draw(shape);
    }

    _window->display();
    sf::sleep(sf::seconds(0.01));


  }
  return -1;
}

int OptionMenu::handleEvent(sf::Event event) {
  for (unsigned int i = 0; i < items.size(); i++) {
    if(items[i]->HandleEvent(event))
      return Screen::NONE;
  }

    switch (event.type) {
        case sf::Event::Closed:
            return Screen::EXIT;
            break;
        case sf::Event::KeyPressed:
            return onKeyPressed(event);
            break;
        case sf::Event::Resized: 
            this->resize(event.size.width, event.size.height);
            return Screen::NONE;
            break;
        case sf::Event::MouseMoved:
            onMouseMoved(event);
            return Screen::NONE;
            break;
        case sf::Event::MouseButtonReleased:
            return onMouseButtonReleased(event);
            break;
        default:
            return Screen::NONE;
            break;
    }
}

int OptionMenu::onKeyPressed(sf::Event event) {
  switch (event.key.code) {
  case sf::Keyboard::Up:
    selectedItem = (selectedItem + items.size() -1) % items.size();
    return Screen::NONE;
    break;
  case sf::Keyboard::Down:
    selectedItem = (selectedItem + 1) % items.size();
    return Screen::NONE;
    break;
  case sf::Keyboard::Return:{
    int rtn = items[selectedItem]->DoAction();    
    if(rtn == ACTION_SAVE) {      
      sf::Vector3i col = hexToCol(_color->GetText());
      if(col != sf::Vector3i(-1,-1,-1))
	game->UpdatePlayerInfo(_name->GetText(), col);
      else
	game->UpdatePlayerInfo(_name->GetText(), game->getColor());
	 
      rtn = Screen::MAINMENU;
    }
    return rtn;
    break;
  }
  case sf::Keyboard::Escape:
    return Screen::GAME;
    break;
  default:
    return Screen::NONE;
    break;
  }
}

void OptionMenu::resize(int width, int height) {
    _window->setView(sf::View(sf::FloatRect(0, 0, width, height)));
    for (unsigned int i = 0; i < items.size(); i++)
        items[i]->CenterX(width);
}

void OptionMenu::onMouseMoved(sf::Event event) {
    int x = event.mouseMove.x;
    int y = event.mouseMove.y;

    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i]->GetBbox().contains(x, y)) {
            selectedItem = i;
            break;
        }
    }
}

int OptionMenu::onMouseButtonReleased(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      for (unsigned int i = 0; i < items.size(); i++) {
	if (items[i]->GetBbox().contains(mouse->getPosition().x, mouse->getPosition().y)) {
	  return items[i]->DoAction();
	}
      }
    }
    return Screen::NONE;
}

