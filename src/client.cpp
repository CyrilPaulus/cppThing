#include "config.h"
#include "client.h"

Client::Client(sf::RenderWindow *window, ImageManager *imageManager){
  this->window = window;
  this->imageManager = imageManager;
  ticker = new Ticker();
  worldDisplay = new sf::RenderTexture();
  worldDisplay->Create(window->GetWidth(), window->GetHeight());
  mouse = new Mouse(window, worldDisplay, imageManager);
  world = new World(imageManager);
  addCube = false;
  removeCube = false;
  player = new Player(world, imageManager);
  zoom = 1;
}

Client::~Client(){
  delete player;
  delete ticker;
  delete mouse;
  delete worldDisplay;
  delete world;
}

int Client::Run(){
  running = true;
  sf::Event event;
  while(running) {
    while(window->PollEvent(event)) {
      HandleEvent(event);
    }

    if(ticker->Tick()){
      Update(ticker->GetElapsedSeconds());
    }

    mouse->Update();

    Draw();

  }
  return 1;
}

void Client::HandleEvent(sf::Event event) {
  switch (event.Type) {
  case sf::Event::Closed:
    OnClose();
    break;
  case sf::Event::MouseButtonPressed:
    OnMouseButtonPressed(event);
    break;
  case sf::Event::MouseButtonReleased:
    OnMouseButtonReleased(event);
  default:
    break;
  }
}

void Client::OnClose() {
  running = false;
}

void Client::OnMouseButtonPressed(sf::Event event){
  switch(event.MouseButton.Button){
  case sf::Mouse::Left:
    addCube = true;
    break;
  case sf::Mouse::Right:
    removeCube = true;
    break;
  default:
    break;
  }
}

void Client::OnMouseButtonReleased(sf::Event event){
  switch(event.MouseButton.Button){
  case sf::Mouse::Left:
    addCube = false;
    break;
  case sf::Mouse::Right:
    removeCube = false;
    break;
  default:
    break;
  }
}


void Client::Update(float frametime) {
  if (addCube)
    world->AddCube(mouse->GetWorldPosition(), 1);

  if (removeCube)
    world->RemoveCube(mouse->GetWorldPosition());

  Input input;
  input.Left = sf::Keyboard::IsKeyPressed(sf::Keyboard::Left);
  input.Right = sf::Keyboard::IsKeyPressed(sf::Keyboard::Right);
  input.Up = sf::Keyboard::IsKeyPressed(sf::Keyboard::Up);
  input.Down = sf::Keyboard::IsKeyPressed(sf::Keyboard::Down);
  player->Update(frametime, input);
  player->SetEyesPosition(mouse->GetWorldPosition());
}

void Client::Draw() {
  UpdateView();
  window->Clear(GameConstant::BackgroundColor);

  worldDisplay->Clear(sf::Color(0,0,0,0));
  world->Draw(worldDisplay);
  player->Draw(worldDisplay);
  worldDisplay->Display();

  window->Draw(sf::Sprite(worldDisplay->GetTexture()));
  mouse->Draw(window);
  window->Display();
}


void Client::UpdateView() {
  sf::View newView = worldDisplay->GetView();
  float left = newView.GetCenter().x - newView.GetSize().x / 2;
  float right = newView.GetCenter().x + newView.GetSize().x / 2;
  if (player->GetBbox().Left - 100 * zoom < left)
    newView.Move(sf::Vector2f(player->GetBbox().Left - 100 * zoom - left, 0));
  else if (player->GetBbox().Left + player->GetBbox().Width +100 * zoom > right)
    newView.Move(sf::Vector2f(player->GetBbox().Left + player->GetBbox().Width + 100 * zoom - right, 0));

  float top = newView.GetCenter().y - newView.GetSize().y / 2;
  float bottom = newView.GetCenter().y + newView.GetSize().y / 2;
  if (player->GetBbox().Top - 100 * zoom < top)
    newView.Move(sf::Vector2f(0, player->GetBbox().Top - 100 * zoom - top));
  else
    newView.Move(sf::Vector2f(0, player->GetBbox().Top + player->GetBbox().Height + 100 * zoom - bottom));

  worldDisplay->SetView(newView);
}

