#include <SDL/SDL.h>
#include "SDLMain.h"
#include <SDL_image/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include "SDL_collide.h"
#include "GameObject.h"
#include "Grant.h"
#include "Timer.h"
#include "RenderingUtils.h"

using namespace std;

ColorKeyRGB colorKeyScreen;


int main(int argc, char ** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        return 1;
    }
    
    SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    
    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    SDL_Surface *background = RenderingUtils::load_image(BACKGROUND_IMG);
	TileMap* tileMap = new TileMap;
    tileMap->loadMap();
    
    SDL_Event event;
    bool quit = false;
    Timer fps;
    Grant grant;
	grant.init(screen,&camera,tileMap);

    vector<GameObject*> listGameObjects(2);
    listGameObjects.push_back(&grant);
    
    if(RenderingUtils::init(screen) == false)
    {
        return 1;
    }
    
    SDL_WM_SetCaption("My Game",NULL);
    
    while( quit == false )
    {
        fps.start();
        
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
            
            // process input
            for(vector<GameObject*>::iterator it = listGameObjects.begin(); it != listGameObjects.end(); ++it) {
                GameObject* obj = *it;
                if(obj)
                {
                    obj->handleInput(&event);
                }
            }
        }
        
        RenderingUtils::fill_colorkey(screen);
        
        //Show the background
        //RenderingUtils::apply_surface( 0, 0, background, screen, &camera );

        tileMap->drawMap(screen,&camera);
        
        // logic of the game
        
        // move
        for(vector<GameObject*>::iterator it = listGameObjects.begin(); it != listGameObjects.end(); ++it) {
            GameObject* obj = *it;
            if(obj)
            {
                obj->move(listGameObjects);
                if(obj->isCameraFocused())
                {
                    obj->setCamera();
                }
                obj->show();
            }
        }
        
        // end of logic of the game
        
        RenderingUtils::update(screen);
        
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
        
    }
    
    
    RenderingUtils::clean_up(screen);
    
    //Return
    return 0;
}

