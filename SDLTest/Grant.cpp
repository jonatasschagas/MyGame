//
//  Grant.cpp
//  SDLTest
//
//  Created by Jonatas Chagas on 26/10/2013.
//  Copyright (c) 2013 Jonatas Chagas. All rights reserved.
//

#include "Grant.h"
#include "RenderingUtils.h"

Grant::Grant()
{
    x = 0, y = 0, w = 40, h = 45, offset = 0,velocity = 0,xVel = 0, yVel = 0, currentAction = ACTION_RIGHT;
    cameraFocused = true;
    
    Animation* animationUp = new Animation;
    animationUp->image = RenderingUtils::load_image(GRANT_UP);
    animationUp->currentFrame = 0;
    animationUp->action = ACTION_UP;
    animationUp->numberOfFrames = 8;
    for(int i = 0; i <= animationUp->numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        clip->y = 0;
        animationUp->listClips.push_back(clip);
    }
    listAnimations.push_back(animationUp);
    
    Animation* animationDown = new Animation;
    animationDown->image = RenderingUtils::load_image(GRANT_DOWN);
    animationDown->currentFrame = 0;
    animationDown->action = ACTION_DOWN;
    animationDown->numberOfFrames = 8;
    for(int i = 0; i <= animationDown->numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        clip->y = 0;
        animationDown->listClips.push_back(clip);
    }
    listAnimations.push_back(animationDown);
    
    Animation* animationLeft = new Animation;
    animationLeft->image = RenderingUtils::load_image(GRANT_LEFT);
    animationLeft->currentFrame = 0;
    animationLeft->action = ACTION_LEFT;
    animationLeft->numberOfFrames = 8;
    for(int i = 0; i <= animationLeft->numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        clip->y = 0;
        animationLeft->listClips.push_back(clip);
    }
    listAnimations.push_back(animationLeft);
    
    Animation* animationRight = new Animation;
    animationRight->image = RenderingUtils::load_image(GRANT_RIGHT);
    animationRight->currentFrame = 0;
    animationRight->action = ACTION_RIGHT;
    animationRight->numberOfFrames = 8;
    for(int i = 0; i <= animationRight->numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        clip->y = 0;
        animationRight->listClips.push_back(clip);
    }
    listAnimations.push_back(animationRight);
    
    Animation* animationRightShoot = new Animation;
    animationRightShoot->image = RenderingUtils::load_image(GRANT_RIGHT_SHOOT);
    animationRightShoot->currentFrame = 0;
    animationRightShoot->action = RIGHT + ACTION_SHOOT;
    animationRightShoot->numberOfFrames = 1;
    for(int i = 0; i <= animationRightShoot->numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        clip->y = 0;
        animationRightShoot->listClips.push_back(clip);
    }
    listAnimations.push_back(animationRightShoot);
    
    Animation* animationUpShoot = new Animation;
    animationUpShoot->image = RenderingUtils::load_image(GRANT_UP_SHOOT);
    animationUpShoot->currentFrame = 0;
    animationUpShoot->action = UP + ACTION_SHOOT;
    animationUpShoot->numberOfFrames = 1;
    for(int i = 0; i <= animationUpShoot->numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        clip->y = 0;
        animationUpShoot->listClips.push_back(clip);
    }
    listAnimations.push_back(animationUpShoot);
    
    Animation* animationLeftShoot = new Animation;
    animationLeftShoot->image = RenderingUtils::load_image(GRANT_LEFT_SHOOT);
    animationLeftShoot->currentFrame = 0;
    animationLeftShoot->action = LEFT + ACTION_SHOOT;
    animationLeftShoot->numberOfFrames = 1;
    for(int i = 0; i <= animationLeftShoot->numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        clip->y = 0;
        animationLeftShoot->listClips.push_back(clip);
    }
    listAnimations.push_back(animationLeftShoot);

    Animation* animationDownShoot = new Animation;
    animationDownShoot->image = RenderingUtils::load_image(GRANT_DOWN_SHOOT);
    animationDownShoot->currentFrame = 0;
    animationDownShoot->action = DOWN + ACTION_SHOOT;
    animationDownShoot->numberOfFrames = 1;
    for(int i = 0; i <= animationDownShoot->numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        clip->y = 0;
        animationDownShoot->listClips.push_back(clip);
    }
    listAnimations.push_back(animationDownShoot);

    
}
