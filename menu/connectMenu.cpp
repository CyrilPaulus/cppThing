#include "connectMenu.h"
#include "../config.h"

int ConnectMenu_Connect() {
  return Screen::GAME;
}

int ConnectMenu_Cancel() {
  return Screen::MAINMENU;
}

ConnectMenu::ConnectMenu(sf::RenderWindow* w, ImageManager* img, Game* game) : Screen(w, img) {
    r = new Renderer(img);
    mouse = new Mouse(w, w, img);
    items.push_back(new MenuItem("Connect", sf::Vector2f(0, 130), &ConnectMenu_Connect));
    items.push_back(new MenuItem("Cancel", sf::Vector2f(0, 160), &ConnectMenu_Cancel));

    selectedItem = 0;

    for (unsigned int i = 0; i < items.size(); i++)
        items[i]->CenterX(window->getSize().x);
    running = true;
}

ConnectMenu::~ConnectMenu() {
  for(unsigned int i = 0; i < items.size(); i++)
    delete items[i];
  delete mouse;
  delete r;
}

int ConnectMenu::run() {
  resize(window->getSize().x, window->getSize().y);
  sf::Event event;
  while (running) {
    while (window->pollEvent(event)) {
      int rtn = handleEvent(event);
      if (rtn != Screen::NONE)
	return rtn;
    }
    
    mouse->update();
    
    window->clear(sf::Color(100, 149, 237));
    
    for (unsigned int i = 0; i < items.size(); i++) {
      items[i]->Draw(window, i == selectedItem);
    }
    mouse->draw(window);
    window->display();
    sf::sleep(sf::seconds(0.01));
  }
  return -1;
}

int ConnectMenu::handleEvent(sf::Event event) {
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

int ConnectMenu::onKeyPressed(sf::Event event) {
  switch (event.key.code) {
  case sf::Keyboard::Up:
    selectedItem = (selectedItem + items.size() -1) % items.size();
    return Screen::NONE;
    break;
  case sf::Keyboard::Down:
    selectedItem = (selectedItem + 1) % items.size();
    return Screen::NONE;
    break;
  case sf::Keyboard::Return:
    return items[selectedItem]->DoAction();
    break;
  case sf::Keyboard::Escape:
    return Screen::GAME;
    break;
  default:
    return Screen::NONE;
    break;
  }
}

void ConnectMenu::resize(int width, int height) {
    window->setView(sf::View(sf::FloatRect(0, 0, width, height)));
    for (unsigned int i = 0; i < items.size(); i++)
        items[i]->CenterX(width);
}

void ConnectMenu::onMouseMoved(sf::Event event) {
    int x = event.mouseMove.x;
    int y = event.mouseMove.y;

    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i]->GetBbox().contains(x, y)) {
            selectedItem = i;
            break;
        }
    }
}

int ConnectMenu::onMouseButtonReleased(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      for (unsigned int i = 0; i < items.size(); i++) {
	if (items[i]->GetBbox().contains(mouse->getPosition().x, mouse->getPosition().y)) {
	  return items[i]->DoAction();
	}
      }
    }
    return Screen::NONE;
}

