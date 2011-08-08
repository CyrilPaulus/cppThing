#include "config.h"
#include "client.h"

Client::Client(sf::RenderWindow *window, ImageManager *imageManager){
  this->window = window;
  this->imageManager = imageManager;
  ticker = new Ticker();
  ui = new sf::RenderImage();
  ui->Create(window->GetWidth(), window->GetHeight());
  mouse = new Mouse(window, imageManager);
  world = new World(imageManager);
  addCube = false;
  removeCube = false;
  player = new Player(world, imageManager);
}

Client::~Client(){
  delete player;
  delete ticker;
  delete mouse;
  delete ui;
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
  if(addCube)
    world->AddCube(mouse->GetWorldPosition(), 1);
  
  if(removeCube)
    world->RemoveCube(mouse->GetWorldPosition());

  player->SetEyesPosition(mouse->GetWorldPosition());
}

void Client::Draw() {
  window->Clear(GameConstant::BackgroundColor);
  
  //UI DRAWING
  ui->Clear(sf::Color(0,0,0,0));
  mouse->Draw(ui);
  ui->Display();
  //WORLD DRAWING
  world->Draw(window);
  player->Draw(window);
  window->Draw(sf::Sprite(ui->GetImage()));  
  window->Display();
}
