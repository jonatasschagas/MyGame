//
//  TileMap.cpp
//  SDLTest
//
//  Created by Jonatas Chagas on 26/10/2013.
//  Copyright (c) 2013 Jonatas Chagas. All rights reserved.
//

#include "TileMap.h"
#include "SDL_collide.h"
#include "RenderingUtils.h"
#include <iostream>
#include <fstream>

using namespace std;

TileMap::TileMap()
{}

void TileMap::loadMap()
{
    tileSheet = RenderingUtils::load_image(TILEMAP_SHEET);
    
    TileType* tileWhite = new TileType;
    tileWhite->collidable = 0;
    tileWhite->box = new SDL_Rect;
    tileWhite->box->x = 0;
    tileWhite->box->y = 0;
    tileWhite->box->w = 50;
    tileWhite->box->h = 50;
    tileWhite->tileType = 0;
    
    TileType* tileBlack = new TileType;
    tileBlack->collidable = 1;
    tileBlack->box = new SDL_Rect;
    tileBlack->box->x = 50;
    tileBlack->box->y = 0;
    tileBlack->box->w = 50;
    tileBlack->box->h = 50;
    tileBlack->tileType = 1;
    
    tileTypes.push_back(tileWhite);
    tileTypes.push_back(tileBlack);
    
    int x = 0, y = 0;
    
    ifstream map(TILEMAP_FILE);
    
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        int tileTypeId = -1;
        
        map >> tileTypeId;
        
        if( map.fail() == true )
        {
            map.close();
            return;
        }
        
        if( ( tileTypeId >= 0 ) && ( tileTypeId < TILE_SPRITES ) )
        {
            TileType* tileType = getTileById(tileTypeId);
            Tile* tile = new Tile;
            tile->x = x;
            tile->y = y;
            tile->tileType = tileType;
            tiles.push_back(tile);
        }
        else
        {
            map.close();
            return;
        }
        
        x += TILE_WIDTH;
        
        if( x >= LEVEL_WIDTH )
        {
            x = 0;
            
            y += TILE_HEIGHT;
        }
    }
    
}

void TileMap::drawMap(SDL_Surface* screen)
{
    for(vector<Tile*>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
        Tile* tile = *it;
        if(tile)
        {
            RenderingUtils::apply_surface(tile->x,tile->y,tileSheet,screen,tile->tileType->box);
        }
    }
}

bool TileMap::collide(int x,int y,int w, int h)
{
    for(vector<Tile*>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
        Tile* tile = *it;
        if(tile && tile->tileType->collidable)
        {
            SDL_Rect* box = tile->tileType->box;
            
            int result = SDL_CollideBoundingBox(x,y,w,h,
                                                box->x , box->y, box->w, box->h);
            
            if(result != 0)
            {
                // collided...
                return true;
            }
        }
    }
    return false;
}

TileType* TileMap::getTileById(int tileTypeId)
{
    for(vector<TileType*>::iterator it = tileTypes.begin(); it != tileTypes.end(); ++it) {
        TileType* tileType = *it;
        if(tileType && tileType->tileType == tileTypeId)
        {
            return tileType;
        }
    }
    return NULL;
}