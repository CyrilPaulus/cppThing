#include "config.h"
#include "client.h"

#include "network/cubeupdate.h"
#include "network/usermessage.h"
Client::Client(sf::RenderWindow *window, ImageManager *imageManager, ZoidCom* zcom){
  this->window = window;
  //TODO find why we can't use the same imageManager.
  this->imageManager = new ImageManager();
  this->zcom = zcom;
  ticker = new Ticker();
  ticker->SetUpdateRate(GameConstant::UPDATE_RATE);
  worldDisplay = new sf::RenderTexture();
  worldDisplay->Create(window->GetWidth(), window->GetHeight());
  mouse = new Mouse(window, worldDisplay, imageManager);
  world = new World(this, imageManager, false);
  addCube = false;
  removeCube = false;
  player = NULL;
  zoom = 1;
  cubeType = 0;
  displayCube = new Cube(imageManager, cubeType);
  displayCube->SetPosition(sf::Vector2f(window->GetWidth() - 10 - Cube::WIDTH, 
					window->GetHeight() - 10 - Cube::HEIGHT));
  pseudo = "Anon";
  layer = 1;
}

Client::~Client(){
  delete displayCube;
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
      Update(ticker->GetElapsedMilliSeconds());
    } else {
      if(zcom)
	zcom->Sleep(1);
    }
    
    mouse->Update();
    
    Draw();
    
  }
  ZCom_Disconnect(serverId, NULL);
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
  switch(event.Key.Code) {
  case sf::Keyboard::A:
    layer = (layer + GameConstant::LAYERNBR - 1) % GameConstant::LAYERNBR;
    printf("%d\n", layer);
    break;
  case sf::Keyboard::Z:
    layer = (layer + 1) % GameConstant::LAYERNBR;
    printf("%d\n", layer);
    break;
  default:
    break;
  }
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

void Client::OnResized(sf::Event event){
  sf::View newView = sf::View(sf::FloatRect(0, 0, event.Size.Width, event.Size.Height));
  window->SetView(newView);
  
  worldDisplay->Create(event.Size.Width, event.Size.Height);
  newView = worldDisplay->GetDefaultView();
  newView.SetCenter(player->GetCenter());
  newView.Zoom(zoom);
  worldDisplay->SetView(newView);

 displayCube->SetPosition(sf::Vector2f(window->GetWidth() - 10 - Cube::WIDTH, 
					window->GetHeight() - 10 - Cube::HEIGHT));

}

void Client::OnMouseWheelMoved(sf::Event event) {
  if(event.MouseWheel.Delta < 0)
    cubeType = (cubeType - 1 + Cube::BLOCKTYPECOUNT) % Cube::BLOCKTYPECOUNT;
  else
    cubeType = (cubeType + 1) % Cube::BLOCKTYPECOUNT;
  displayCube->SetType(cubeType);
}


void Client::Update(unsigned int frametime) {
  this->ZCom_processReplicators(frametime);
  this->ZCom_processInput();
  
  if (addCube && world->CanAddCube(mouse->GetWorldPosition(), layer)){
    ZCom_BitStream *message = new ZCom_BitStream();
    CubeUpdate cu(cubeType, mouse->GetWorldPosition(), true, layer);
    cu.Encode(message);
    ZCom_sendData(serverId, message);	
  }
    
  if (removeCube && world->CanRemoveCube(mouse->GetWorldPosition(), layer)){
    ZCom_BitStream *message = new ZCom_BitStream();
    CubeUpdate cu(cubeType, mouse->GetWorldPosition(), false, layer);
    cu.Encode(message);
    ZCom_sendData(serverId, message);	
  }

  Input input;
  input.Left = sf::Keyboard::IsKeyPressed(sf::Keyboard::Left);
  input.Right = sf::Keyboard::IsKeyPressed(sf::Keyboard::Right);
  input.Up = sf::Keyboard::IsKeyPressed(sf::Keyboard::Up);
  input.Down = sf::Keyboard::IsKeyPressed(sf::Keyboard::Down);
  
  world->Update();
  world->UpdatePlayer(frametime, input);
  
  if(player != NULL) {
    player->SetEyesPosition(mouse->GetWorldPosition());
    UpdateView();
  }
  
  this->ZCom_processOutput();
}

void Client::Draw() {
  window->Clear(GameConstant::BackgroundColor);

  worldDisplay->Clear(sf::Color(0,0,0,0));
  world->Draw(worldDisplay);
  worldDisplay->Display();

  window->Draw(sf::Sprite(worldDisplay->GetTexture()));
  displayCube->Draw(window);
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
  else if(player->GetBbox().Top + player->GetBbox().Height + 100 * zoom > bottom)
    newView.Move(sf::Vector2f(0, player->GetBbox().Top + player->GetBbox().Height + 100 * zoom - bottom));

  worldDisplay->SetView(newView);
}

//Zoidcom handling

void Client::ZCom_cbConnectResult(ZCom_ConnID id, eZCom_ConnectResult result, ZCom_BitStream &reply) {
  if (result == eZCom_ConnAccepted){
    printf("Connection established, launching...\n");
    clientId = reply.getInt(32);
    std::string newPseudo(reply.getStringStatic());
    pseudo = newPseudo;
    ZCom_requestZoidMode(serverId, 1);
    this->Run();
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

void Client::Connect() {
  this->ZCom_setDebugName("Client");
  if(!this->ZCom_initSockets(true, 0, 0)) {
    printf("Can't init client socket\n");
    exit(255);
  }

  ZCom_Address server;
  server.setAddress(eZCom_AddressUDP, 0, "localhost:50645");
  ZCom_BitStream *connectionInfo = new ZCom_BitStream();
  connectionInfo->addString(pseudo.data());
  serverId = this->ZCom_Connect(server, connectionInfo);
  if(serverId == ZCom_Invalid_ID){
    printf("Invalid id\n");
    exit(255);
  }
  this->Run();
}

