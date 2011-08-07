#include "config.h"
#include "client.h"

Client::Client(sf::RenderWindow *window, ImageManager *imageManager){
  this->window = window;
  this->imageManager = imageManager;
  ticker = new Ticker();
  ui = new sf::RenderImage();
  ui->Create(window->GetWidth(), window->GetHeight());
  mouse = new Mouse(window, imageManager);
  ent = new Entity(imageManager);
}

Client::~Client(){
  delete ticker;
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

  default:
    break;
  }
}

void Client::OnClose() {
  running = false;
} 

void Client::Update(float frametime) {
  
}

void Client::Draw() {
  window->Clear(GameConstant::BackgroundColor);
  
  //UI DRAWING
  ui->Clear(sf::Color(0,0,0,0));
  mouse->Draw(ui);
  ui->Display();
  //WORLD DRAWING
  ent->Draw(window);
  window->Draw(sf::Sprite(ui->GetImage()));  
  window->Display();
}
