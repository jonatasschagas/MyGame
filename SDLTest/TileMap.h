//
//  TileMap.h
//  SDLTest
//
//  Created by Jonatas Chagas on 26/10/2013.
//  Copyright (c) 2013 Jonatas Chagas. All rights reserved.
//

#ifndef __SDLTest__TileMap__
#define __SDLTest__TileMap__

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include "SDLMain.h"
#include <SDL_image/SDL_image.h>
#include <iostream>

using namespace std;

const string TILEMAP_FILE = "/Users/jonataschagas/Documents/images/map.map";
const string TILEMAP_SHEET = "/Users/jonataschagas/Documents/images/tile_sheet.jpg";
const int TILE_WIDTH = 50;
const int TILE_HEIGHT = 50;
const int TOTAL_TILES = 192;
const int TILE_SPRITES = 2;

// 0 for white
// 1 for black
struct TileType
{
    SDL_Rect* box;
    bool collidable;
    int tileType;
};

struct Tile
{
    TileType* tileType;
    int x;
    int y;
};

class TileMap
{
private:
    SDL_Surface* tileSheet;
    vector<TileType*> tileTypes;
    vector<Tile*> tiles;
public:
    TileMap();  
    void loadMap();
	void drawMap(SDL_Surface* screen);
    bool collide(int x,int y,int w, int h);
    TileType* getTileById(int tileType);
};

#endif /* defined(__SDLTest__TileMap__) */
