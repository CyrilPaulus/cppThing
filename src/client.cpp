#include "config.h"
#include "client.h"

Client::Client(sf::RenderWindow *window, ImageManager *imageManager){
  this->window = window;
  this->imageManager = imageManager;
  ui = new sf::RenderImage();
  ui->Create(window->GetWidth(), window->GetHeight());
  mouse = new Mouse(window, imageManager);
}

Client::~Client(){
  delete mouse;
  delete ui;
}

int Client::Run(){
  running = true;
  sf::Event event;
  while(running) {
    while(window->PollEvent(event)) {
      HandleEvent(event);
    }


    Update();
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

  default:
    break;
  }
}

void Client::OnClose() {
  running = false;
} 

void Client::Update() {
}

void Client::Draw() {
  window->Clear(GameConstant::BackgroundColor);
  
  //UI DRAWING
  ui->Clear(GameConstant::BackgroundColor);
  mouse->Draw(ui);
  ui->Display();
  //WORLD DRAWING
  window->Draw(sf::Sprite(ui->GetImage()));  
  window->Display();
}
