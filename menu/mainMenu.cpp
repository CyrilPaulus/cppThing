#include "mainMenu.h"
#include "../config.h"

int LocalGame() {
  return Screen::GAME;
}

int Connect() {
  return Screen::CONNECT;
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
    r = new Renderer(img);
    mouse = new Mouse(w, w, img);
    items.push_back(new MenuItem("Local Game", sf::Vector2f(0, 100), &LocalGame));
    items.push_back(new MenuItem("Connect", sf::Vector2f(0, 130), &Connect));
    items.push_back(new MenuItem("Options", sf::Vector2f(0, 160), &Option));

    if(game->getType() == Game::LOCAL) {
      items.push_back(new MenuItem("Load Map", sf::Vector2f(0, 190), &LoadMap));
      items.push_back(new MenuItem("Save Map", sf::Vector2f(0, 220), &SaveMap));
    }

    items.push_back(new MenuItem("Exit", sf::Vector2f(0, 280), &Exit));
    selectedItem = 0;

    for (unsigned int i = 0; i < items.size(); i++)
        items[i]->CenterX(_window->getSize().x);

    p = new Player(NULL);
    pImage = new sf::RenderTexture();
    pImage->create(_window->getSize().x, _window->getSize().y);
    sf::View v = pImage->getDefaultView();
    v.zoom(0.08);
    v.setCenter(p->getCenter() + sf::Vector2f(0, -30));
    pImage->setView(v);
    running = true;

    background = sf::Color::Black;
    nextColor = sf::Color::Black;
}

MainMenu::~MainMenu() {
  for(unsigned int i = 0; i < items.size(); i++)
    delete items[i];
  delete p;
  delete pImage;
  delete mouse;
  delete r;
}

int MainMenu::run() {
    p->setColor(sf::Vector3i(rand() % 255, rand() % 255, rand() % 255));
    resize(_window->getSize().x, _window->getSize().y);
    sf::Event event;
    while (running) {
        while (_window->pollEvent(event)) {
            int rtn = handleEvent(event);
            if (rtn != Screen::NONE)
                return rtn;
        }

        updateColor();
        mouse->update();

        _window->clear(background);
        pImage->clear(background);
        r->renderPlayer(p, pImage);
        pImage->display();

        sf::Sprite pSprite(pImage->getTexture());
        pSprite.setPosition(sf::Vector2f((_window->getSize().x - pImage->getSize().x) / 2
                , _window->getSize().y - pImage->getSize().y));
        _window->draw(pSprite);
        for (unsigned int i = 0; i < items.size(); i++) {
	  items[i]->Draw(_window);
	  items[i]->SetActive(i == selectedItem);
        }
        mouse->draw(_window);
        _window->display();
        sf::sleep(sf::seconds(0.01));
    }

    return -1;
}

int MainMenu::handleEvent(sf::Event event) {
  for (unsigned int i = 0; i < items.size(); i++) {
    if(items[i]->HandleEvent(event))
      return Screen::NONE;
  }

    switch (event.type) {
        case sf::Event::Closed:
            return Screen::EXIT;
            break;
        case sf::Event::KeyPressed:
            return onKeyPressed(event);
            break;
        case sf::Event::Resized:
            this->resize(event.size.width, event.size.height);
            return Screen::NONE;
            break;
        case sf::Event::MouseMoved:
            onMouseMoved(event);
            return Screen::NONE;
            break;
        case sf::Event::MouseButtonReleased:
            return onMouseButtonReleased(event);
            break;
        default:
            return Screen::NONE;
            break;
    }
}

int MainMenu::onKeyPressed(sf::Event event) {
  switch (event.key.code) {
  case sf::Keyboard::Up:
    selectedItem = (selectedItem + items.size() -1) % items.size();
    return Screen::NONE;
    break;
  case sf::Keyboard::Down:
    selectedItem = (selectedItem + 1) % items.size();
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

void MainMenu::resize(int width, int height) {
    _window->setView(sf::View(sf::FloatRect(0, 0, width, height)));
    for (unsigned int i = 0; i < items.size(); i++)
        items[i]->CenterX(width);
}

void MainMenu::onMouseMoved(sf::Event event) {
    int x = event.mouseMove.x;
    int y = event.mouseMove.y;

    p->setEyesPosition(pImage->mapPixelToCoords(sf::Vector2i(x, y)));
    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i]->GetBbox().contains(x, y)) {
            selectedItem = i;
            break;
        }
    }
}

int MainMenu::onMouseButtonReleased(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      for (unsigned int i = 0; i < items.size(); i++) {
	if (items[i]->GetBbox().contains(mouse->getPosition().x, mouse->getPosition().y)) {
	  return items[i]->DoAction();
	}
      }
    }
    return Screen::NONE;
}

void MainMenu::updateColor() {

    if (background == nextColor) {
        nextColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    }

    if (background.r > nextColor.r)
        background.r -= 1;
    else if (background.r < nextColor.r)
        background.r += 1;

    if (background.g > nextColor.g)
        background.g -= 1;
    else if (background.g < nextColor.g)
        background.g += 1;

    if (background.b > nextColor.b)
        background.b -= 1;
    else if (background.b < nextColor.b)
        background.b += 1;
}
