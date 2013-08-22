#include "config.h"
#include "client.h"

#include <iostream>
#include <stdexcept>
#include "network/CubeUpdate.h"
#include "network/UserMessage.h"
#include "network/ClientToken.h"
#include "network/PlayerAdd.h"
#include "network/PlayerDelete.h"
#include "network/PlayerUpdate.h"
#include "network/TextMessage.h"

Client::Client(sf::RenderWindow *window, ImageManager *imageManager) : Screen(window, imageManager) {
 
  ticker = new Ticker();
  ticker->setUpdateRate(GameConstant::UPDATE_RATE);
  worldDisplay = new sf::RenderTexture();

  //TODO define resolution variable in client so we don't need to call window
  worldDisplay->create(window->getSize().x, window->getSize().y);
  mouse = new Mouse(window, worldDisplay, imageManager);
  world = new World(false);
  addCube = false;
  removeCube = false;
  player = new Player(world);
  world->addPlayer(player);
  zoom = 1;
  cubeType = 0;
  
  renderer = new Renderer(imageManager);
  
  //TODO wrap this in a function
  displayCube = new CubeDisplay(imageManager);
  displayCube->SetType(cubeType);
  sf::Vector2f uiPosition = sf::Vector2f(window->getSize().x - 10 - Cube::WIDTH
					 ,window->getSize().y - 10 - Cube::HEIGHT); 
  displayCube->SetPosition(uiPosition);

  _chat_box = new ChatBox();
  _chat_box->SetPosition(sf::Vector2f(10, window->getSize().y - 10));

  layerDisplay = new LayerDisplay(imageManager, GameConstant::LAYERNBR);
  layerDisplay->SetPosition(uiPosition + sf::Vector2f(-5, - 5 - layerDisplay->GetSize().y));

  pseudo = "Anon";
  player->setPseudo(pseudo);
  layer = 1;
  layerDisplay->SetLayer(layer);
  port = 50645;
  ip = "localhost";
  mainMenu = false;
  id = -1;

  if(enet_initialize() != 0) {
    throw std::runtime_error("ENet initialization failed");
  }
  
  client = enet_host_create(NULL, 1, 2, 0, 0);
  
  if(client == NULL){
    throw std::runtime_error("ENet client initialization failed");
  }
  server = NULL;
  connected = false;
  _has_focus = true;
}

Client::~Client(){
  enet_host_destroy(client);
  enet_deinitialize();
  delete renderer;
  delete displayCube;
  delete _chat_box;
  delete layerDisplay;
  delete ticker;
  delete mouse;
  delete worldDisplay;
  delete world;
}

int Client::run(){
  running = true;
  sf::Event event;
  while(running) {
    while(window->pollEvent(event)) {
      handleEvent(event);
    }
    
    if(mainMenu){
      mainMenu = false;
      return Screen::MAINMENU;
    }
    
    if(ticker->tick()){
      update(ticker->getElapsedTime());
    }else {
      sf::sleep(sf::seconds(0.01));
    }
    
    mouse->update();
    
    draw();
    
  }
  if(connected)
    disconnect();
  return -1;
}

void Client::handleEvent(sf::Event event) {
  switch (event.type) {
  case sf::Event::Closed:
    onClose();
    break;
  case sf::Event::MouseButtonPressed:
    onMouseButtonPressed(event);
    break;
  case sf::Event::MouseButtonReleased:
    onMouseButtonReleased(event);
    break;
  case sf::Event::MouseWheelMoved:
    onMouseWheelMoved(event);
    break;
  case sf::Event::KeyPressed:
    onKeyPressed(event);
    break;
  case sf::Event::Resized:
    onResized(event);
    break;
  case sf::Event::GainedFocus:
    _has_focus = true;
    break;
  case sf::Event::LostFocus:
    _has_focus = false;
    break;
  default:
    break;
  }
  _chat_box->handleEvent(event);
}

void Client::onClose() {
  running = false;
}

void Client::onKeyPressed(sf::Event event) {
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

void Client::onMouseButtonPressed(sf::Event event){
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

void Client::onMouseButtonReleased(sf::Event event){
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

void Client::onResized(sf::Event event){
  sf::View newView = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
  window->setView(newView);
  
  worldDisplay->create(event.size.width, event.size.height);
  newView = worldDisplay->getDefaultView();

  if(player != NULL)
    newView.setCenter(player->getCenter());
  
  newView.zoom(zoom);
  worldDisplay->setView(newView);
  
  sf::Vector2f uiPosition = sf::Vector2f(window->getSize().x - 10 - Cube::WIDTH
					 ,window->getSize().y - 10 - Cube::HEIGHT);
  displayCube->SetPosition(uiPosition);
  layerDisplay->SetPosition(uiPosition + sf::Vector2f(-5, - 5 - layerDisplay->GetSize().y));
  _chat_box->SetPosition(sf::Vector2f(10, window->getSize().y - 10));
}

void Client::onMouseWheelMoved(sf::Event event) {
  if(event.mouseWheel.delta < 0)
    cubeType = (cubeType - 1 + Cube::BLOCKTYPECOUNT) % Cube::BLOCKTYPECOUNT;
  else
    cubeType = (cubeType + 1) % Cube::BLOCKTYPECOUNT;
  displayCube->SetType(cubeType);
}


void Client::update(sf::Time frametime) {

  if(server != NULL) {
    ENetEvent event;
    while(enet_host_service(client, &event, 0) > 0) {
      switch(event.type) {
      case ENET_EVENT_TYPE_CONNECT:{
	std::cout << "Connection to server established" << std::endl;
	this->connected = true;
	break;
      }
      case ENET_EVENT_TYPE_DISCONNECT:
	std::cout << "Connection to server lost" << std::endl;
	this->connected = false;
	enet_peer_reset(server);
	break;
      case ENET_EVENT_TYPE_RECEIVE:{
	sf::Packet p;
	p.append(event.packet->data, event.packet->dataLength);
	handlePacket(p);
	enet_packet_destroy(event.packet);
	break;
      }
      default:
	break;
      }
    }
  }
  
  if (addCube && world->canAddCube(mouse->getWorldPosition(), layer)){
    //TODO send cube update pkt
    CubeUpdate cu(cubeType, mouse->getWorldPosition(), true, layer);
    sendReliable(&cu);
    //world->AddCube(cu.GetPosition(), cu.GetCubeType(), cu.GetLayer());
  }
    
  if (removeCube && world->canRemoveCube(mouse->getWorldPosition(), layer)){
    CubeUpdate cu(cubeType, mouse->getWorldPosition(), false, layer);
    sendReliable(&cu);
    //world->RemoveCube(cu.GetPosition(), cu.GetLayer());

    //TODO send cube update pkt;	
  }

  Input input;
  input.Left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && _has_focus;
  input.Right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && _has_focus;
  input.Up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && _has_focus;
  input.Down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && _has_focus;

  //TODO send player update
  world->update();
  //world->UpdatePlayer(frametime, input);
  player->update(frametime, input);
  if(player != NULL) {
    player->setEyesPosition(mouse->getWorldPosition());
    updateView();
  }
  PlayerUpdate up;
  up.setPosition(player->getPosition());
  up.setEyePosition(mouse->getWorldPosition());
  up.setId(id);
  send(&up);  

  while(_chat_box->getPendingCount() > 0) {
    TextMessage tm;
    tm.setMessage(_chat_box->popPendingMsg());
    sendReliable(&tm);
  }
}

void Client::draw() {
    sf::Color clearC(GameConstant::BackgroundColor.x, GameConstant::BackgroundColor.y, GameConstant::BackgroundColor.z);
  window->clear(clearC);

  worldDisplay->clear(sf::Color(0,0,0,0));
  renderer->renderWorld(world, worldDisplay);
  worldDisplay->display();

  window->draw(sf::Sprite(worldDisplay->getTexture()));
  displayCube->Draw(window);
  _chat_box->Draw(window);
  layerDisplay->Draw(window);
  mouse->draw(window);
  window->display();
}


void Client::updateView() {
  sf::View newView = worldDisplay->getView();
  float left = newView.getCenter().x - newView.getSize().x / 2;
  float right = newView.getCenter().x + newView.getSize().x / 2;
  if (player->getBbox().left - 100 * zoom < left)
    newView.move(sf::Vector2f(player->getBbox().left - 100 * zoom - left, 0));
  else if (player->getBbox().left + player->getBbox().width +100 * zoom > right)
    newView.move(sf::Vector2f(player->getBbox().left + player->getBbox().width + 100 * zoom - right, 0));

  float top = newView.getCenter().y - newView.getSize().y / 2;
  float bottom = newView.getCenter().y + newView.getSize().y / 2;
  if (player->getBbox().top - 100 * zoom < top)
    newView.move(sf::Vector2f(0, player->getBbox().top - 100 * zoom - top));
  else if(player->getBbox().top + player->getBbox().height + 100 * zoom > bottom)
    newView.move(sf::Vector2f(0, player->getBbox().top + player->getBbox().height + 100 * zoom - bottom));
  
  worldDisplay->setView(newView);
}

void Client::sendReliable(Packet* p) {
  sf::Packet data = p->encode();
  ENetPacket* packet = enet_packet_create(data.getData(), data.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(server, 0, packet);
}

void Client::send(Packet* p) {
  sf::Packet data = p->encode();
  ENetPacket* packet = enet_packet_create(data.getData(), data.getDataSize(), 0);
  enet_peer_send(server, 1, packet);
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

void Client::connect() {
  
  //Connect to server
  ENetAddress address;
  enet_address_set_host(&address, ip.c_str());
  address.port = port;

  std::cout << "Connecting to server\n" << std::endl;
  server = enet_host_connect(client, &address, 2, 0);
  if(server == NULL) {
    throw std::runtime_error("ENet connection creation failed");
  }
}

void Client::disconnect() {
  
  std::cout << "Disconnecting" << std::endl;
  ENetEvent event;
  enet_peer_disconnect(server, 0);
  while(enet_host_service(client, &event, 3000) > 0) {
    switch(event.type) {
    case ENET_EVENT_TYPE_RECEIVE:
      enet_packet_destroy(event.packet);
      break;
    case ENET_EVENT_TYPE_DISCONNECT:
      std::cout << "Disconnected from server" << std::endl;
      return;
    default:
      break;
    }
  }

  //Force disconnect
  enet_peer_reset(server);
  
}

void Client::setPort(int port){
  this->port = port;
}

void Client::setIp(std::string ip){
  this->ip = ip;
}

void Client::handlePacket(sf::Packet p) {
  sf::Uint8 type;
  p >> type;
  switch(type) {
  case Packet::CubeUpdate:{
    CubeUpdate cu;
    cu.decode(p);
    if(cu.GetAdded())
      world->addCube(cu.GetPosition(), cu.GetCubeType(), cu.GetLayer(), true);
    else
      world->removeCube(cu.GetPosition(), cu.GetLayer());
    break;
  }
  case Packet::UserMessage:{
    break;
  }
  case Packet::ClientConnect: {
    ClientToken cc;
    cc.decode(p);
    this->id = cc.getId();
    player->setId(id);
    std::cout << "server assigned id:" << id << std::endl;
    
    //Now we can send the client info
    PlayerAdd pa;
    pa.setColor(player->getColor());
    pa.setPseudo(player->getPseudo());
    pa.setId(id);
    sendReliable(&pa);
    break;
  }
  case Packet::AddPlayer: {
    PlayerAdd ap;
    ap.decode(p);
    if(id != ap.getId()) {
      Player* p = new Player(world);
      p->setColor(ap.getColor());
      p->setPseudo(ap.getPseudo());
      p->setId(ap.getId());
      world->addPlayer(p);
    } else {
      setPseudo(ap.getPseudo());
    }
    break;
  }
  case Packet::DeletePlayer: {
    PlayerDelete dp;
    dp.decode(p);
    world->removePlayerById(dp.getId());
  }
  case Packet::UpdatePlayer: {
    PlayerUpdate up;
    up.decode(p);
    if(up.getId() != id) {
     Player* p = world->getPlayerById(up.getId());
      if(p != NULL) {
	p->setPosition(up.getPosition());
	p->setEyesPosition(up.getEyePosition());
      }
    }
    break;
  }
  case Packet::TextMessage: {
    TextMessage tm;
    tm.decode(p);
    _chat_box->addMessage(tm.getMessage());
    break;
  }    
  default: 
    break;
  }
}

void Client::setPseudo(std::string p) {
  pseudo = p;
  player->setPseudo(p);
}
