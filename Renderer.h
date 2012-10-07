/* 
 * File:   Renderer.h
 * Author: cyril
 *
 * Created on September 23, 2012, 4:49 PM
 */

#ifndef RENDERER_H
#define	RENDERER_H

#include <SFML/Graphics.hpp>
#include "imageManager.h"
#include "entity/world.h"
#include "entity/cube.h"
#include "entity/player.h"


class Renderer {
public:
    Renderer(ImageManager* imgManager);
    Renderer(const Renderer& orig);
    virtual ~Renderer();
    
    void renderWorld(World* w, sf::RenderTarget* rt);
    void renderCube(Cube* b, sf::RenderTarget* rt);
    void renderPlayer(Player* p, sf::RenderTarget* rt);
    
private:
    ImageManager* imgManager;

};

#endif	/* RENDERER_H */

