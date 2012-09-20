#include "config.h"
#include "client.h"

#include <iostream>
#include "network/cubeupdate.h"
#include "network/usermessage.h"

Client::Client(sf::RenderWindow *window, ImageManager *imageManager) : Screen(window, imageManager) {
 
  ticker = new Ticker();
  ticker->SetUpdateRate(GameConstant::UPDATE_RATE);
  worldDisplay = new sf::RenderTexture();

  //TODO define resolution variable in client so we don't need to call window
  worldDisplay->create(window->getSize().x, window->getSize().y);
  mouse = new Mouse(window, worldDisplay, imageManager);
  world = new World(imageManager, false);
  addCube = false;
  removeCube = false;
  player = new Player(imageManager, world);
  world->AddPlayer(player);
  zoom = 1;
  cubeType = 0;
  
  //TODO wrap this in a function
  displayCube = new CubeDisplay(imageManager);
  displayCube->SetType(cubeType);
  sf::Vector2f uiPosition = sf::Vector2f(window->getSize().x - 10 - Cube::WIDTH
					 ,window->getSize().y - 10 - Cube::HEIGHT); 
  displayCube->SetPosition(uiPosition);
  layerDisplay = new LayerDisplay(imageManager, GameConstant::LAYERNBR);
  layerDisplay->SetPosition(uiPosition + sf::Vector2f(0, - 5 - layerDisplay->GetSize().y));
  pseudo = "Anon";
  layer = 1;
  layerDisplay->SetLayer(layer);
  port = 50645;
  ip = "localhost";
  mainMenu = false;
}

Client::~Client(){
  delete displayCube;
  delete layerDisplay;
  delete ticker;
  delete mouse;
  delete worldDisplay;
  delete world;
}

int Client::Run(){
  running = true;
  sf::Event event;
  while(running) {
    while(window->pollEvent(event)) {
      HandleEvent(event);
    }
    
    if(mainMenu){
      mainMenu = false;
      return Screen::MAINMENU;
    }
    
    if(ticker->Tick()){
      Update(ticker->GetElapsedTime());
    }     
    
    mouse->Update();
    
    Draw();
    
  }
  return -1;
}

void Client::HandleEvent(sf::Event event) {
  switch (event.type) {
  case sf::Event::Closed:
    OnClose();
    break;
  case sf::Event::MouseButtonPressed:
    OnMouseButtonPressed(event);
    break;
  case sf::Event::MouseButtonReleased:
    OnMouseButtonReleased(event);
    break;
  case sf::Event::MouseWheelMoved:
    OnMouseWheelMoved(event);
    break;
  case sf::Event::KeyPressed:
    OnKeyPressed(event);
    break;
  case sf::Event::Resized:
    OnResized(event);
  default:
    break;
  }
}

void Client::OnClose() {
  running = false;
}

void Client::OnKeyPressed(sf::Event event) {
  switch(event.key.code) {
  case sf::Keyboard::A:
    layer = (layer + GameConstant::LAYERNBR - 1) % GameConstant::LAYERNBR;
    layerDisplay->SetLayer(layer);
    break;
  case sf::Keyboard::Z:
    layer = (layer + 1) % GameConstant::LAYERNBR;
    layerDisplay->SetLayer(layer);
    break;
  case sf::Keyboard::Escape:
    mainMenu = true;
    break;
  default:
    break;
  }
}

void Client::OnMouseButtonPressed(sf::Event event){
  switch(event.mouseButton.button){
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
  switch(event.mouseButton.button){
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

void Client::OnResized(sf::Event event){
  sf::View newView = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
  window->setView(newView);
  
  worldDisplay->create(event.size.width, event.size.height);
  newView = worldDisplay->getDefaultView();

  if(player != NULL)
    newView.setCenter(player->GetCenter());
  
  newView.zoom(zoom);
  worldDisplay->setView(newView);
  
  displayCube->SetPosition(sf::Vector2f(window->getSize().x - 10 - Cube::WIDTH, 
					window->getSize().y - 10 - Cube::HEIGHT));
  
}

void Client::OnMouseWheelMoved(sf::Event event) {
  if(event.mouseWheel.delta < 0)
    cubeType = (cubeType - 1 + Cube::BLOCKTYPECOUNT) % Cube::BLOCKTYPECOUNT;
  else
    cubeType = (cubeType + 1) % Cube::BLOCKTYPECOUNT;
  displayCube->SetType(cubeType);
}


void Client::Update(sf::Time frametime) {
  
  if (addCube && world->CanAddCube(mouse->GetWorldPosition(), layer)){
    //TODO send cube update pkt
    CubeUpdate cu(cubeType, mouse->GetWorldPosition(), true, layer);
    world->AddCube(cu.GetPosition(), cu.GetCubeType(), cu.GetLayer());
  }
    
  if (removeCube && world->CanRemoveCube(mouse->GetWorldPosition(), layer)){
    CubeUpdate cu(cubeType, mouse->GetWorldPosition(), false, layer);
    world->RemoveCube(cu.GetPosition(), cu.GetLayer());

    //TODO send cube update pkt;	
  }

  Input input;
  input.Left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
  input.Right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
  input.Up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
  input.Down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

  //TODO send player update
  world->Update();
  world->UpdatePlayer(frametime, input);
  
  if(player != NULL) {
    player->SetEyesPosition(mouse->GetWorldPosition());
    UpdateView();
  }
  
}

void Client::Draw() {
  window->clear(GameConstant::BackgroundColor);

  worldDisplay->clear(sf::Color(0,0,0,0));
  world->Draw(worldDisplay);
  worldDisplay->display();

  window->draw(sf::Sprite(worldDisplay->getTexture()));
  displayCube->Draw(window);
  layerDisplay->Draw(window);
  mouse->Draw(window);
  window->display();
}


void Client::UpdateView() {
  sf::View newView = worldDisplay->getView();
  float left = newView.getCenter().x - newView.getSize().x / 2;
  float right = newView.getCenter().x + newView.getSize().x / 2;
  if (player->GetBbox().left - 100 * zoom < left)
    newView.move(sf::Vector2f(player->GetBbox().left - 100 * zoom - left, 0));
  else if (player->GetBbox().left + player->GetBbox().width +100 * zoom > right)
    newView.move(sf::Vector2f(player->GetBbox().left + player->GetBbox().width + 100 * zoom - right, 0));

  float top = newView.getCenter().y - newView.getSize().y / 2;
  float bottom = newView.getCenter().y + newView.getSize().y / 2;
  if (player->GetBbox().top - 100 * zoom < top)
    newView.move(sf::Vector2f(0, player->GetBbox().top - 100 * zoom - top));
  else if(player->GetBbox().top + player->GetBbox().height + 100 * zoom > bottom)
    newView.move(sf::Vector2f(0, player->GetBbox().top + player->GetBbox().height + 100 * zoom - bottom));
  
  worldDisplay->setView(newView);
}


/*
void Client::ZCom_cbConnectResult(ZCom_ConnID id, eZCom_ConnectResult result, ZCom_BitStream &reply) {
  if (result == eZCom_ConnAccepted){
    printf("Connection established, launching...\n");
    clientId = reply.getInt(32);
    std::string newPseudo(reply.getStringStatic());
    pseudo = newPseudo;
    ZCom_requestZoidMode(serverId, 1);
  }
  else
    printf("Connection failed");
}

void Client::ZCom_cbConnectionClosed(ZCom_ConnID id, eZCom_CloseReason reason, ZCom_BitStream &reasondada) {
  printf("Disconnected from server\n");
  running = false;
}

void Client::ZCom_cbZoidResult( ZCom_ConnID id, eZCom_ZoidResult result, zU8 new_level, ZCom_BitStream &reason ) {
  if(result == eZCom_ZoidEnabled)
    printf("[CLIENT] %d level entered", new_level);
  else
    printf("[CLIENT] access refused to %d level", new_level);
}

void Client::ZCom_cbNodeRequest_Dynamic( ZCom_ConnID id, ZCom_ClassID requested_class, ZCom_BitStream *announcedata, eZCom_NodeRole role, ZCom_NodeID net_id ) {

  if(requested_class == Cube::GetClass(false)){
    int type = announcedata->getInt(32);
    float x = announcedata->getFloat(23);
    float y = announcedata->getFloat(23);
    int layerIndex = announcedata->getInt(8);
    world->AddCube(sf::Vector2f(x,y), type, layerIndex, true);
  }
  else if (requested_class == Player::GetClass(false)) {
    ZCom_ConnID idIn = announcedata->getInt(32);
    float x = announcedata->getFloat(23);
    float y = announcedata->getFloat(23);
    int r = announcedata->getInt(8);
    int g = announcedata->getInt(8);
    int b = announcedata->getInt(8);
    Player* p = new Player(imageManager, world);
    //p->SetPosition(sf::Vector2f(x, y));
    p->SetColor(sf::Color(r, g, b));
    p->RegisterZCom(this, false);
    
    if(role == eZCom_RoleOwner)
      player = p;
    world->AddPlayer(p);
  }
}
*/

void Client::Connect() {
  
  //TODO connect to server
  std::stringstream saddress;
  saddress<<ip;
  saddress<<":";
  saddress<<port;
     
}

void Client::SetPort(int port){
  this->port = port;
}

void Client::SetIp(std::string ip){
  this->ip = ip;
}
