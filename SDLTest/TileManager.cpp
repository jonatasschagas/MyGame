//
//  TileManager.cpp
//  SDLTest
//
//  Created by Jonatas Chagas on 26/10/2013.
//  Copyright (c) 2013 Jonatas Chagas. All rights reserved.
//

#include "TileManager.h"
#include "RenderingUtils.h"

TileManager::TileManager()
{
    tilesWidth = 50;
    tilesHeight = 50;
    tileSprites = 2;
    tileSheet = RenderingUtils::load_image(TILES_SHEET);
    
    // Unfortunatelly we have to set the tiles
    // manually until we manage to find a system
    
    SDL_Rect* tileSpriteRect = new SDL_Rect;
    tileSpriteRect->x = 0;
    tileSpriteRect->y = 0;
    tileSpriteRect->w = tilesWidth;
    tileSpriteRect->h = tilesHeight;
    TileType* tileType = new TileType(tileSpriteRect,0,false);
    tileTypes.push_back(tileType);
    
    SDL_Rect* tileSpriteRect2 = new SDL_Rect;
    tileSpriteRect2->x = tilesWidth;
    tileSpriteRect2->y = 0;
    tileSpriteRect2->w = tilesWidth;
    tileSpriteRect2->h = tilesHeight;
    TileType* tileType2 = new TileType(tileSpriteRect2,1,true);
    tileTypes.push_back(tileType2);
    
}

TileType* TileManager::getTileByType(int tileType)
{
    for(vector<TileType*>::iterator it = tileTypes.begin(); it != tileTypes.end(); ++it) {
        TileType* obj = *it;
        if(obj && obj->getType() == tileType)
        {
            return obj;
        }
    }
    return NULL;
}

vector<TileType*> TileManager::getTiles()
{
    return tileTypes;
}

SDL_Surface* TileManager::getTileSheet()
{
    return tileSheet;
}

int TileManager::getTilesWidth()
{
    return tilesWidth;
}

int TileManager::getTilesHeight()
{
    return tilesHeight;
}

int TileManager::getTilesSprites()
{
    return tileSprites;
}
