#include "mainMenu.h"
#include "../config.h"

MainMenu::MainMenu(sf::RenderWindow* w, ImageManager* img, Game* game) : Screen(w, img) {
  itemCount = 6;
  items = (MenuItem**) malloc(itemCount * sizeof(MenuItem*));
  items[0] = new MenuItem("Local Game", sf::Vector2f(0, 100), NULL);
  items[1] = new MenuItem("Connect", sf::Vector2f(0, 130), NULL);
  items[2] = new MenuItem("Options", sf::Vector2f(0, 160), NULL);
  items[3] = new MenuItem("Load Map", sf::Vector2f(0, 190), NULL);
  items[4] = new MenuItem("Save Map", sf::Vector2f(0, 220), NULL);
  items[5] = new MenuItem("Exit", sf::Vector2f(0, 280), NULL);
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

  while(running) {

    window->Clear(sf::Color(100, 149, 237));
    for(int i = 0; i < itemCount; i++) {
      items[i]->Draw(window, i == selectedItem);
    }
    window->Display();
    sf::Sleep(0.01);
  }

  return -1;
}


