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
  mouse = new Mouse(w, w, img);
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
  
  p = new Player(imageManager, NULL);
  pImage = new sf::RenderTexture();
  pImage->Create(window->GetWidth(), window->GetHeight());
  sf::View v = pImage->GetDefaultView();
  v.Zoom(0.08);
  v.SetCenter(p->GetCenter() + sf::Vector2f(0, -30));
  pImage->SetView(v);
  running = true;

  background = sf::Color::Black;
  nextColor = sf::Color::Black;
}

MainMenu::~MainMenu() {
  for(int i = 0; i < itemCount; i++)
    delete items[i];
  delete p;
  delete pImage;
  delete mouse;
  delete items;
}

int MainMenu::Run() {
  p->SetColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
  Resize(window->GetWidth(), window->GetHeight());
  sf::Event event;
  while(running) {
    while(window->PollEvent(event)) {
      int rtn = HandleEvent(event);
      if(rtn != Screen::NONE)
	return rtn;
    }
    
    UpdateColor();
    mouse->Update();

    window->Clear(background);
    pImage->Clear(background);
    p->Draw(pImage);
    pImage->Display();

    sf::Sprite pSprite(pImage->GetTexture());
    pSprite.SetPosition(sf::Vector2f((window->GetWidth() - pImage->GetWidth()) / 2
				     , window->GetHeight() - pImage->GetHeight()));
    window->Draw(pSprite);
    for(int i = 0; i < itemCount; i++) {
      items[i]->Draw(window, i == selectedItem);
    }
    mouse->Draw(window);
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
  case sf::Event::Resized:
    this->Resize(event.Size.Width, event.Size.Height);
    return Screen::NONE;
    break;
  case sf::Event::MouseMoved:
    OnMouseMoved(event);
    return Screen::NONE;
    break;
  case sf::Event::MouseButtonReleased:
    return OnMouseButtonReleased(event);
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
    return Screen::GAME;
    break;
  default:
    return Screen::NONE;
    break;
  }
}

void MainMenu::Resize(int width, int height) {
  window->SetView(sf::View(sf::FloatRect(0, 0, width, height)));
  for(int i = 0; i < itemCount; i++)
    items[i]->CenterX(width);
}

void MainMenu::OnMouseMoved(sf::Event event) {
  int x = event.MouseMove.X;
  int y = event.MouseMove.Y;

  p->SetEyesPosition(pImage->ConvertCoords(x, y));
  for(int i = 0; i < itemCount; i++) {
    if(items[i]->GetBbox().Contains(x, y)) {
	selectedItem = i;
	break;
      }
  }
}

int MainMenu::OnMouseButtonReleased(sf::Event event) {
  if(event.MouseButton.Button == sf::Mouse::Left) {
    for(int i = 0; i < itemCount; i++) {
      if(items[i]->GetBbox().Contains(mouse->GetPosition().x, mouse->GetPosition().y)) {
	return items[i]->DoAction();
      }
    }
  }
  return Screen::NONE;
}

void MainMenu::UpdateColor() {

  if(background == nextColor) {
    nextColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
  }

  if(background.r > nextColor.r)
    background.r -= 1;
  else if(background.r < nextColor.r)
    background.r += 1;

  if(background.g > nextColor.g)
    background.g -= 1;
  else if(background.g < nextColor.g)
    background.g += 1;

  if(background.b > nextColor.b)
    background.b -= 1;
  else if(background.b < nextColor.b)
    background.b += 1;
}
