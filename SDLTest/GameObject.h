//
//  GameObject.h
//  SDLTest
//
//  Created by Jonatas Chagas on 26/10/2013.
//  Copyright (c) 2013 Jonatas Chagas. All rights reserved.
//

#ifndef __SDLTest__GameObject__
#define __SDLTest__GameObject__

#include "SDLMain.h"
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include "SDL_collide.h"
#include "TileMap.h"
#include <iostream>
#include <vector>

using namespace std;

const int ACTION_UP    = 0;
const int ACTION_DOWN  = 1;
const int ACTION_LEFT  = 2;
const int ACTION_RIGHT = 3;
const int ACTION_SHOOT = 4;

const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;

// each animation has a name
// and a list of clips
struct Animation {
    int action;
    SDL_Surface* image;
    int numberOfFrames;
    int currentFrame;
    vector<SDL_Rect*> listClips;
};

class GameObject
{
protected:
    int x, y, w, h, offset,velocity,xVel, yVel,currentAction,direction;
    bool cameraFocused = false, isMoving = false;
    vector<Animation*> listAnimations;
	SDL_Rect* camera;
    SDL_Surface* screen;
	TileMap* tileMap;
public:
    GameObject();
	void init(SDL_Surface* screen,SDL_Rect* camera,TileMap* tileMap);
    virtual void handleInput(SDL_Event* event);
    virtual void move(vector<GameObject*> list);
    virtual void show();
    // sets camera over the game object
    virtual void setCamera();
    Animation* getAnimation(int action);
    int getX();
    int getY();
    int getW();
    int getH();
    bool isCameraFocused();
};

#endif /* defined(__SDLTest__GameObject__) */
