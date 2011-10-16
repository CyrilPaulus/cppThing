#include "mainMenu.h"
#include "../config.h"

int LocalGame() {
  return Screen::GAME;
}

int Connect() {
  return Screen::NONE;
}

int Option() {
  return Screen::NONE;
}

int LoadMap() {
  return Screen::NONE;
}

int SaveMap() {
  return Screen::NONE;
}

int Exit() {
  return Screen::EXIT;
}

MainMenu::MainMenu(sf::RenderWindow* w, ImageManager* img, Game* game) : Screen(w, img) {
  itemCount = 6;
  items = (MenuItem**) malloc(itemCount * sizeof(MenuItem*));
  items[0] = new MenuItem("Local Game", sf::Vector2f(0, 100), &LocalGame);
  items[1] = new MenuItem("Connect", sf::Vector2f(0, 130), &Connect);
  items[2] = new MenuItem("Options", sf::Vector2f(0, 160), &Option);
  items[3] = new MenuItem("Load Map", sf::Vector2f(0, 190), &LoadMap);
  items[4] = new MenuItem("Save Map", sf::Vector2f(0, 220), &SaveMap);
  items[5] = new MenuItem("Exit", sf::Vector2f(0, 280), &Exit);
  selectedItem = 0;
  
  for(int i = 0; i < itemCount; i++)
    items[i]->CenterX(window->GetWidth());
  
  running = true;
}

MainMenu::~MainMenu() {
  for(int i = 0; i < itemCount; i++)
    delete items[i];
  delete items;
}

int MainMenu::Run() {

  sf::Event event;
  while(running) {
    while(window->PollEvent(event)) {
      int rtn = HandleEvent(event);
      if(rtn != Screen::NONE)
	return rtn;
    }

    window->Clear(sf::Color(100, 149, 237));
    for(int i = 0; i < itemCount; i++) {
      items[i]->Draw(window, i == selectedItem);
    }
    window->Display();
    sf::Sleep(0.01);
  }

  return -1;
}


int MainMenu::HandleEvent(sf::Event event) {
  switch (event.Type) {
  case sf::Event::Closed:
    return Screen::EXIT;
    break;
  case sf::Event::KeyPressed:
    return OnKeyPressed(event);
    break;
  default:
    return Screen::NONE;
    break;
  }
}

int MainMenu::OnKeyPressed(sf::Event event) {
  switch(event.Key.Code) {
  case sf::Keyboard::Up:
    selectedItem = (selectedItem + itemCount - 1) % itemCount;
    return Screen::NONE;
    break;
  case sf::Keyboard::Down:
    selectedItem = (selectedItem + 1) % itemCount;
    return Screen::NONE;
    break;
  case sf::Keyboard::Return:
    return items[selectedItem]->DoAction();
    break;
  case sf::Keyboard::Escape:
    printf("Back to game\n");
    return Screen::GAME;
    break;
  default:
    return Screen::NONE;
    break;
  }
}
