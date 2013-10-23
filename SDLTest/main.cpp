#include <SDL/SDL.h>
#include "SDLMain.h"
#include <SDL_image/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include "SDL_collide.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 20;

using namespace std;
////////////////////////////////////////

#define SDL_COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define SDL_COLLIDE_MIN(a,b)	((a < b) ? a : b)

// http://sdl-collide.sourceforge.net

void fill_blankscreen(SDL_Surface* screen)
{
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );
}

SDL_Surface *load_image( string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
    }
    
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

void clean_up(SDL_Surface *image)
{
    SDL_FreeSurface(image);
    SDL_Quit();
}

bool init(SDL_Surface* screen)
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    
    if( screen == NULL )
    {
        return false;
    }
    
    SDL_WM_SetCaption( "Event test", NULL );
    
    //If everything initialized fine
    return true;
}

bool update(SDL_Surface* screen)
{
    if( SDL_Flip( screen ) == -1 )
    {
        return false;
    }
    return true;
}


////////////////////////////////////////
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

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    started = false;
    paused = false;
}

int Timer::get_ticks()
{
    if( started == true )
    {
        if( paused == true )
        {
            return pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}

void Timer::pause()
{
    if( ( started == true ) && ( paused == false ) )
    {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    if( paused == true )
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}
/////////////////////////////////////////////////////////////

class GameObject
{
protected:
    int x, y, w, h, offset,velocity,frame,status,numberOfFrames,xVel, yVel;
    SDL_Surface* image;
    vector<SDL_Rect*> listClips;
    
public:
    GameObject();
    virtual void handle_input(SDL_Event* event);
    virtual void move(vector<GameObject*> list);
    virtual void show(SDL_Surface* screen);
    SDL_Surface* getImage();
    int getX();
    int getY();
    int getW();
    int getH();
};

GameObject::GameObject()
{
    //Initialize the offsets
    x = 0;
    y = 0;
    
    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}

SDL_Surface* GameObject::getImage()
{
    return image;
}

int GameObject::getX()
{
    return x;
}

int GameObject::getY()
{
    return y;
}

int GameObject::getW()
{
    return w;
}

int GameObject::getH()
{
    return h;
}

void GameObject::handle_input(SDL_Event* event)
{
    if( event->type == SDL_KEYDOWN )
    {
        switch( event->key.keysym.sym )
        {
            case SDLK_UP: yVel -= h / 2; break;
            case SDLK_DOWN: yVel += h / 2; break;
            case SDLK_LEFT: xVel -= w / 2; break;
            case SDLK_RIGHT: xVel += w / 2; break;
        }
    }
}

void GameObject::move(vector<GameObject*> list)
{
    int oX = x;
    int oY = y;
    
    x += xVel;
    
    if( ( x < 0 ) || ( x + w > SCREEN_WIDTH ) )
    {
        //move back
        x -= xVel;
    }
    
    //Move the dot up or down
    y += yVel;
    
    //If the dot went too far up or down
    if( ( y < 0 ) || ( y + h > SCREEN_HEIGHT ) )
    {
        //move back
        y -= yVel;
    }
    
    for(vector<GameObject*>::iterator it = list.begin(); it != list.end(); ++it) {
        GameObject* obj = *it;
        if(obj && obj != this)
        {
            int result = SDL_CollideBoundingBox(this->x , this->y, this->w, this->h,
                                 obj->getX() , obj->getY(),obj->getW(), obj->getH());
            
            if(result != 0)
            {
                // collided...
                x = oX;
                y = oY;
                return;
            }

        }
    }
    
}

void GameObject::show(SDL_Surface* screen)
{
    if(frame >= numberOfFrames)
    {
        frame = 0;
    }
    else
    {
        frame++;
    }
    
    cout << frame << endl;
    
    //Show the dot
    if(listClips.size() > 0)
    {
        apply_surface( x, y, image, screen,listClips[frame]);
    }
    else
    {
        apply_surface( x, y, image, screen,NULL);
    }

}

/////////////////////////////////////////////////////////////////////////////

const string SONIC_MESH = "/Users/jonataschagas/Documents/images/sonic_running.jpg";
class Sonic: public GameObject
{
    public:
        Sonic();
};

Sonic::Sonic()
{
    image = load_image(SONIC_MESH);
    numberOfFrames = 8;
    frame = 0;
    offset = 0;
    velocity = 0;
    w = 100;
    h = 100;
    for(int i = 0; i <= numberOfFrames; i++)
    {
        SDL_Rect* clip = new SDL_Rect;
        clip->h = h;
        clip->w = w;
        clip->x = i * w;
        
        cout << clip->x << endl;
        
        clip->y = 0;
        listClips.push_back(clip);
    }
}

/////////////////////////////////////////////////////////////////////////////

const string WALL_MESH = "/Users/jonataschagas/Documents/images/wall.bmp";
class Wall: public GameObject
{
public:
    Wall();
    virtual void move();
    virtual void handle_input(SDL_Event* event);
};

Wall::Wall()
{
    image = load_image(WALL_MESH);
    w = 50;
    h = 400;
    x = 400;
    y = 50;
    frame = 0;
    numberOfFrames = 0;
}

void Wall::move(){}
void Wall::handle_input(SDL_Event* event){}

/////////////////////////////////////////////////////////////////////////////


int main(int argc, char ** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        return 1;
    }
    
    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    SDL_Event event;
    bool quit = false;
    Timer fps;
    Sonic sonic;
    Wall wall;
    vector<GameObject*> listGameObjects(2);
    listGameObjects.push_back(&sonic);
    listGameObjects.push_back(&wall);
    
    if(init(screen) == false)
    {
        return 1;
    }
    
    SDL_WM_SetCaption("Hello World",NULL);
    
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
                    obj->handle_input(&event);
                }
            }
        }
        
        fill_blankscreen(screen);
        
        // logic of the game
        
        // move
        for(vector<GameObject*>::iterator it = listGameObjects.begin(); it != listGameObjects.end(); ++it) {
            GameObject* obj = *it;
            if(obj)
            {
                obj->move(listGameObjects);
                obj->show(screen);
            }
        }
        
        // end of logic of the game
        
        update(screen);
        
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
        
    }
    
    
    clean_up(screen);
    
    //Return
    return 0;
}

