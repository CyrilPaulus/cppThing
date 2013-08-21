/* 
 * File:   Renderer.cpp
 * Author: cyril
 * 
 * Created on September 23, 2012, 4:49 PM
 */

#include "Renderer.h"
#include "config.h"

Renderer::Renderer(ImageManager* imgManager) {
    this->imgManager = imgManager;
    _textFont.loadFromFile("res/arial.ttf");
}

void Renderer::renderWorld(World* world, sf::RenderTarget* rt) {
    for(int i = 0; i < GameConstant::LAYERNBR; i++) {
        std::list<Cube*> cubes = world->getList(i);
        std::list<Cube*>::iterator c;
        for(c=cubes.begin(); c!= cubes.end(); c++)
            renderCube((*c), rt);

        if (i == 0) {
            sf::Vector2f origin = rt->mapPixelToCoords(sf::Vector2i(0, 0));
            sf::RectangleShape fog = sf::RectangleShape(sf::Vector2f(rt->getDefaultView().getSize().x, rt->getDefaultView().getSize().y));
            fog.setPosition(sf::Vector2f(origin.x, origin.y));
            fog.setFillColor(sf::Color(100, 149, 237, 150));
            rt->draw(fog);
        }
        
        if( i== 1) {
            std::list<Player*> players = world->getPlayerList();
            std::list<Player*>::iterator p;
            for(p = players.begin(); p != players.end(); p++) {
                renderPlayer((*p), rt);
            }
        }
    }
}

void Renderer::renderCube(Cube* c, sf::RenderTarget* rt) {
    sf::Sprite sprite;
    sprite.setTexture(*(imgManager->get("tileset")), true);
    int type = c->getType();
    int x = (type % 16) * Cube::WIDTH;
    int y = (type / 16) * Cube::HEIGHT;
    sprite.setTextureRect(sf::IntRect(x, y, Cube::WIDTH, Cube::HEIGHT));
    sprite.setPosition(c->getPosition());
    rt->draw(sprite);
}

void Renderer::renderPlayer(Player* p, sf::RenderTarget* rt) {
    sf::Sprite sprite;
    sprite.setTexture(*(imgManager->get("player")), true);
    sprite.setPosition(p->getPosition());
    sf::Sprite pupil(*(imgManager->get("pupil")));
    sf::Sprite colorMask(*(imgManager->get("colorMask")));    
    rt->draw(sprite);
    pupil.setPosition(p->getPosition() + p->getLeftPupilPosition());
    rt->draw(pupil);
    pupil.setPosition(p->getPosition() + p->getRightPupilPosition());
    rt->draw(pupil);
    colorMask.setPosition(p->getPosition());
    sf::Vector3i color = p->getColor();
    colorMask.setColor(sf::Color(color.x, color.y, color.z));
    rt->draw(colorMask);

    //Time to draw pseudo bitches
    sf::Text text = sf::Text(p->getPseudo(), _textFont, 15);
    text.setPosition(p->getPosition() 
		     - sf::Vector2f(-(p->getBbox().width - text.getLocalBounds().width) / 2
				    , _textFont.getLineSpacing(15)));
    rt->draw(text);
}

Renderer::Renderer(const Renderer& orig) {
}

Renderer::~Renderer() {
}

