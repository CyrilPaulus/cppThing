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
#include "network/UpdatePlayerInfo.h"

Client::Client(sf::RenderWindow *window, ImageManager *image_manager) : Screen(window, image_manager), _thread(&Client::UpdateThread, this)
{

  _ticker = new Ticker();
  _ticker->setUpdateRate(GameConstant::UPDATE_RATE);
  _world_display = new sf::RenderTexture();

  // TODO define resolution variable in client so we don't need to call window
  _world_display->create(window->getSize().x, window->getSize().y);
  _mouse = new Mouse(window, _world_display, _image_manager);
  _world = new World(false);
  _add_cube = false;
  _remove_cube = false;
  _player = new Player(_world);
  _world->addPlayer(_player);
  _zoom = 1;
  _cube_type = 0;

  _renderer = new Renderer(_image_manager);

  // TODO wrap this in a function
  _display_cube = new CubeDisplay(_image_manager);
  _display_cube->SetType(_cube_type);
  sf::Vector2f ui_position = sf::Vector2f(_window->getSize().x - 10 - Cube::WIDTH, _window->getSize().y - 10 - Cube::HEIGHT);
  _display_cube->SetPosition(ui_position);

  _chat_box = new ChatBox();
  _chat_box->SetPosition(sf::Vector2f(10, _window->getSize().y - 10));

  _layer_display = new LayerDisplay(_image_manager, GameConstant::LAYERNBR);
  _layer_display->SetPosition(ui_position + sf::Vector2f(-5, -5 - _layer_display->GetSize().y));

  _pseudo = "Anon";
  _player->setPseudo(_pseudo);
  _layer = 1;
  _layer_display->SetLayer(_layer);
  _port = 50645;
  _ip = "localhost";
  _main_menu = false;
  _id = -1;

  if (enet_initialize() != 0)
  {
    throw std::runtime_error("ENet initialization failed");
  }

  _client = enet_host_create(NULL, 1, 2, 0, 0);

  if (_client == NULL)
  {
    throw std::runtime_error("ENet client initialization failed");
  }
  _server = NULL;
  _connected = false;
  _has_focus = true;

  _connecting = false;
  _connectTime = 0;

  _connectFont.loadFromFile("res/arial.ttf");
  _connectText = sf::Text("Connecting...", _connectFont);
  _notConnectText = sf::Text("Not connected...\nPress ESC for main menu.", _connectFont);
}

Client::~Client()
{
  enet_host_destroy(_client);
  enet_deinitialize();
  delete _renderer;
  delete _display_cube;
  delete _chat_box;
  delete _layer_display;
  delete _ticker;
  delete _mouse;
  delete _world_display;
  delete _world;
}

int Client::run()
{
  _running = true;
  sf::Event event;
  while (_running)
  {

    while (_window->pollEvent(event))
    {
      handleEvent(event);
    }

    if (_main_menu)
    {
      _main_menu = false;
      _running = false;
      return Screen::MAINMENU;
    }

    // Update input there, otherwise it crash...
    input.Left = _running && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && _has_focus;
    input.Right = _running && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && _has_focus;
    input.Up = _running && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && _has_focus;
    input.Down = _running && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && _has_focus;

    sf::sleep(sf::seconds(0.01));

    _mouse->update();
    draw();
  }
  if (_connected)
    disconnect();
  return -1;
}

void Client::StartThread()
{
  _runningThread = true;
  _thread.launch();
}

void Client::StopThread()
{
  _runningThread = false;
  _thread.wait();
}

void Client::UpdateThread()
{

  while (_runningThread)
  {

    if (_ticker->tick())
    {
      update(_ticker->getElapsedTime());
    }
    else
    {
      sf::sleep(sf::seconds(0.01));
    }
  }
}

void Client::handleEvent(sf::Event event)
{
  switch (event.type)
  {
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
    if (!_chat_box->isActive())
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

void Client::onClose()
{
  _running = false;
}

void Client::onKeyPressed(sf::Event event)
{
  switch (event.key.code)
  {
  case sf::Keyboard::A:
    _layer = (_layer + GameConstant::LAYERNBR - 1) % GameConstant::LAYERNBR;
    _layer_display->SetLayer(_layer);
    break;
  case sf::Keyboard::Z:
    _layer = (_layer + 1) % GameConstant::LAYERNBR;
    _layer_display->SetLayer(_layer);
    break;
  case sf::Keyboard::Escape:
    _main_menu = true;
    break;
  default:
    break;
  }
}

void Client::onMouseButtonPressed(sf::Event event)
{
  switch (event.mouseButton.button)
  {
  case sf::Mouse::Left:
    _add_cube = true;
    break;
  case sf::Mouse::Right:
    _remove_cube = true;
    break;
  default:
    break;
  }
}

void Client::onMouseButtonReleased(sf::Event event)
{
  switch (event.mouseButton.button)
  {
  case sf::Mouse::Left:
    _add_cube = false;
    break;
  case sf::Mouse::Right:
    _remove_cube = false;
    break;
  default:
    break;
  }
}

void Client::onResized(sf::Event event)
{
  sf::View newView = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
  _window->setView(newView);

  _world_display->create(event.size.width, event.size.height);
  newView = _world_display->getDefaultView();

  if (_player != NULL)
    newView.setCenter(_player->getCenter());

  newView.zoom(_zoom);
  _world_display->setView(newView);

  sf::Vector2f ui_position = sf::Vector2f(_window->getSize().x - 10 - Cube::WIDTH, _window->getSize().y - 10 - Cube::HEIGHT);
  _display_cube->SetPosition(ui_position);
  _layer_display->SetPosition(ui_position + sf::Vector2f(-5, -5 - _layer_display->GetSize().y));
  _chat_box->SetPosition(sf::Vector2f(10, _window->getSize().y - 10));
}

void Client::onMouseWheelMoved(sf::Event event)
{
  if (event.mouseWheel.delta < 0)
    _cube_type = (_cube_type - 1 + Cube::BLOCKTYPECOUNT) % Cube::BLOCKTYPECOUNT;
  else
    _cube_type = (_cube_type + 1) % Cube::BLOCKTYPECOUNT;
  _display_cube->SetType(_cube_type);
}

void Client::update(sf::Time frametime)
{

  if (_connecting)
  {
    _connectTime += frametime.asSeconds();
    if (_connectTime > CONNECT_TIME_OUT)
    {
      _connecting = false;
      _connected = false;
    }
  }

  if (_server != NULL)
  {
    ENetEvent event;
    while (enet_host_service(_client, &event, 0) > 0)
    {
      switch (event.type)
      {
      case ENET_EVENT_TYPE_CONNECT:
      {
        LOG(INFO) << "Connection to server established";
        _connected = true;
        _connecting = false;
        break;
      }
      case ENET_EVENT_TYPE_DISCONNECT:
        LOG(INFO) << "Connection to server lost";
        _connected = false;
        enet_peer_reset(_server);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
      {
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

  if (_add_cube && _world->canAddCube(_mouse->getWorldPosition(), _layer))
  {
    CubeUpdate cu(_cube_type, _mouse->getWorldPosition(), true, _layer);
    sendReliable(&cu);
  }

  if (_remove_cube && _world->canRemoveCube(_mouse->getWorldPosition(), _layer))
  {
    CubeUpdate cu(_cube_type, _mouse->getWorldPosition(), false, _layer);
    sendReliable(&cu);
  }

  _world->update();
  _player->update(frametime, input);
  if (_player != NULL)
  {
    _player->setEyesPosition(_mouse->getWorldPosition());
    updateView();
  }

  PlayerUpdate up;
  up.setPosition(_player->getPosition());
  up.setEyePosition(_mouse->getWorldPosition());
  up.setId(_id);
  send(&up);

  while (_chat_box->getPendingCount() > 0)
  {
    TextMessage tm;
    tm.setMessage(_chat_box->popPendingMsg());
    sendReliable(&tm);
  }
}

void Client::drawConnecting()
{
  sf::Color clear_color(10, 10, 10);
  _window->clear(clear_color);

  _connectText.setOrigin(_connectText.getLocalBounds().width / 2, _connectText.getLocalBounds().height / 2);
  _connectText.setPosition(_window->getSize().x / 2, _window->getSize().y / 2);
  _connectText.setRotation(_connectTime / CONNECT_TIME_OUT * 360);
  _window->draw(_connectText);

  _mouse->draw(_window);
  _window->display();
}

void Client::drawNotConnected()
{
  sf::Color clear_color(10, 10, 10);
  _window->clear(clear_color);

  _notConnectText.setOrigin(_notConnectText.getLocalBounds().width / 2, _notConnectText.getLocalBounds().height / 2);
  _notConnectText.setPosition(_window->getSize().x / 2, _window->getSize().y / 2);
  _window->draw(_notConnectText);

  _mouse->draw(_window);
  _window->display();
}

void Client::draw()
{

  if (_connecting)
  {
    drawConnecting();
    return;
  }

  if (!_connected)
  {
    drawNotConnected();
    return;
  }

  sf::Color clear_color(GameConstant::BackgroundColor.x, GameConstant::BackgroundColor.y, GameConstant::BackgroundColor.z);
  _window->clear(clear_color);

  _world_display->clear(sf::Color(0, 0, 0, 0));
  _renderer->renderWorld(_world, _world_display);
  _world_display->display();

  _window->draw(sf::Sprite(_world_display->getTexture()));
  _display_cube->Draw(_window);
  _chat_box->Draw(_window);
  _layer_display->Draw(_window);
  _mouse->draw(_window);
  _window->display();
}

void Client::updateView()
{
  sf::View new_view = _world_display->getView();
  float left = new_view.getCenter().x - new_view.getSize().x / 2;
  float right = new_view.getCenter().x + new_view.getSize().x / 2;
  if (_player->getBbox().left - 100 * _zoom < left)
    new_view.move(sf::Vector2f(_player->getBbox().left - 100 * _zoom - left, 0));
  else if (_player->getBbox().left + _player->getBbox().width + 100 * _zoom > right)
    new_view.move(sf::Vector2f(_player->getBbox().left + _player->getBbox().width + 100 * _zoom - right, 0));

  float top = new_view.getCenter().y - new_view.getSize().y / 2;
  float bottom = new_view.getCenter().y + new_view.getSize().y / 2;
  if (_player->getBbox().top - 100 * _zoom < top)
    new_view.move(sf::Vector2f(0, _player->getBbox().top - 100 * _zoom - top));
  else if (_player->getBbox().top + _player->getBbox().height + 100 * _zoom > bottom)
    new_view.move(sf::Vector2f(0, _player->getBbox().top + _player->getBbox().height + 100 * _zoom - bottom));

  _world_display->setView(new_view);
}

void Client::sendReliable(Packet *p)
{
  sf::Packet data = p->encode();
  ENetPacket *packet = enet_packet_create(data.getData(), data.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(_server, 0, packet);
}

void Client::send(Packet *p)
{
  sf::Packet data = p->encode();
  ENetPacket *packet = enet_packet_create(data.getData(), data.getDataSize(), 0);
  enet_peer_send(_server, 1, packet);
}

void Client::connect()
{
  _connected = false;
  _connecting = true;
  _connectTime = 0;
  // Connect to server
  ENetAddress address;
  enet_address_set_host(&address, _ip.c_str());
  address.port = _port;

  LOG(INFO) << "Connecting to server\n";
  _server = enet_host_connect(_client, &address, 2, 0);
  if (_server == NULL)
  {
    throw std::runtime_error("ENet connection creation failed");
  }
}

void Client::disconnect()
{

  LOG(INFO) << "Disconnecting";
  ENetEvent event;
  enet_peer_disconnect(_server, 0);
  while (enet_host_service(_client, &event, 3000) > 0)
  {
    switch (event.type)
    {
    case ENET_EVENT_TYPE_RECEIVE:
      enet_packet_destroy(event.packet);
      break;
    case ENET_EVENT_TYPE_DISCONNECT:
      LOG(INFO) << "Disconnected from server";
      return;
    default:
      break;
    }
  }

  // Force disconnect
  enet_peer_reset(_server);
}

void Client::setPort(int port)
{
  _port = port;
}

void Client::setIp(std::string ip)
{
  _ip = ip;
}

void Client::handlePacket(sf::Packet p)
{
  sf::Uint8 type;
  p >> type;
  switch (type)
  {
  case Packet::CubeUpdate:
  {
    CubeUpdate cu;
    cu.decode(p);
    if (cu.GetAdded())
      _world->addCube(cu.GetPosition(), cu.GetCubeType(), cu.GetLayer(), true);
    else
      _world->removeCube(cu.GetPosition(), cu.GetLayer());
    break;
  }
  case Packet::UserMessage:
  {
    break;
  }
  case Packet::ClientConnect:
  {
    ClientToken cc;
    cc.decode(p);
    _id = cc.getId();
    _player->setId(_id);
    LOG(INFO) << "server assigned id:" << _id;

    // Now we can send the client info
    PlayerAdd pa;
    pa.setColor(_player->getColor());
    pa.setPseudo(_player->getPseudo());
    pa.setId(_id);
    sendReliable(&pa);
    break;
  }
  case Packet::AddPlayer:
  {
    PlayerAdd ap;
    ap.decode(p);
    if (_id != ap.getId())
    {
      Player *p = new Player(_world);
      p->setColor(ap.getColor());
      p->setPseudo(ap.getPseudo());
      p->setId(ap.getId());
      _world->addPlayer(p);
    }
    else
    {
      setPseudo(ap.getPseudo());
    }
    break;
  }
  case Packet::DeletePlayer:
  {
    PlayerDelete dp;
    dp.decode(p);
    _world->removePlayerById(dp.getId());
  }
  case Packet::UpdatePlayer:
  {
    PlayerUpdate up;
    up.decode(p);
    if (up.getId() != _id)
    {
      Player *p = _world->getPlayerById(up.getId());
      if (p != NULL)
      {
        p->setPosition(up.getPosition());
        p->setEyesPosition(up.getEyePosition());
      }
    }
    break;
  }
  case Packet::TextMessage:
  {
    TextMessage tm;
    tm.decode(p);
    _chat_box->addMessage(tm.getMessage());
    break;
  }
  case Packet::UpdatePlayerInfo:
  {
    UpdatePlayerInfo ap;
    ap.decode(p);
    Player *p = _world->getPlayerById(ap.getId());
    p->setColor(ap.getColor());
    p->setPseudo(ap.getPseudo());
    break;
  }
  default:
    break;
  }
}

void Client::setPseudo(std::string p)
{
  _pseudo = p;
  _player->setPseudo(p);
}

std::string Client::getPseudo()
{
  return _player->getPseudo();
}

sf::Vector3i Client::getColor()
{
  return _player->getColor();
}

void Client::setColor(sf::Vector3i color)
{
  _player->setColor(color);
}

void Client::UpdatePlayer(std::string name, sf::Vector3i color)
{

  UpdatePlayerInfo pa;
  pa.setColor(color);
  pa.setPseudo(name);
  pa.setId(_id);
  sendReliable(&pa);
}
