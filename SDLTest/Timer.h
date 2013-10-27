//
//  Timer.h
//  SDLTest
//
//  Created by Jonatas Chagas on 26/10/2013.
//  Copyright (c) 2013 Jonatas Chagas. All rights reserved.
//

#ifndef __SDLTest__Timer__
#define __SDLTest__Timer__

#include <SDL/SDL.h>
#include "SDLMain.h"
#include <SDL_image/SDL_image.h>
#include <iostream>

class Timer
{
private:
    int startTicks;
    int pausedTicks;
    bool paused;
    bool started;
    
public:
    Timer();
    void start();
    void stop();
    void pause();
    void unpause();
    int get_ticks();
    bool is_started();
    bool is_paused();
};


#endif /* defined(__SDLTest__Timer__) */
