//
//  TileManager.h
//  SDLTest
//
//  Created by Jonatas Chagas on 26/10/2013.
//  Copyright (c) 2013 Jonatas Chagas. All rights reserved.
//

#ifndef __SDLTest__TileManager__
#define __SDLTest__TileManager__

#include "TileType.h"
#include "RenderingUtils.h"
#include <iostream>
#include <vector>

using namespace std;

const string TILES_SHEET = "/Users/jonataschagas/Documents/images/tile_sheet.jpg";

class TileManager
{
private:
    int tilesWidth,tilesHeight,tileSprites;
    // image with the tiles
    SDL_Surface* tileSheet;
    // the tiles
    vector<TileType*> tileTypes;
public:
    TileManager();
    SDL_Surface* getTileSheet();
    TileType* getTileByType(int tileType);
    vector<TileType*> getTiles();
    int getTilesWidth();
    int getTilesHeight();
    int getTilesSprites();
};


#endif /* defined(__SDLTest__TileManager__) */
