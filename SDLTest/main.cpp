#include <SDL/SDL.h>
#include "SDLMain.h"
#include <SDL_image/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include "SDL_collide.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 20;
const string BACKGROUND_IMG = "/Users/jonataschagas/Documents/images/grass_bg.jpg";

// the highest the moving speed the slowest the object
// moves on the screen
const int MOVING_SPEED = 5;

struct ColorKeyRGB
{
    int r = 0;
    int g = 0;
    int b = 0;
};

ColorKeyRGB colorKeyScreen;
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

////////////////////////////////////////

#define SDL_COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define SDL_COLLIDE_MIN(a,b)	((a < b) ? a : b)

// http://sdl-collide.sourceforge.net

void fill_colorkey(SDL_Surface* screen)
{
    SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,
                                                         colorKeyScreen.r, colorKeyScreen.g, colorKeyScreen.b));
}

SDL_Surface *load_image( string filename, ColorKeyRGB colorKeyStr )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
        
        if( optimizedImage != NULL )
        {
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, colorKeyStr.r, colorKeyStr.g, colorKeyStr.b );
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
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

const int ACTION_UP    = 0;
const int ACTION_DOWN  = 1;
const int ACTION_LEFT  = 2;
const int ACTION_RIGHT = 3;

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
    int x, y, w, h, offset,velocity,xVel, yVel,currentAction;
    bool cameraFocused = false;
    vector<Animation*> listAnimations;
    
public:
    GameObject();
    virtual void handle_input(SDL_Event* event);
    virtual void move(vector<GameObject*> list);
    virtual void show(SDL_Surface* screen);
    // sets camera over the game object
    virtual void setCamera();
    Animation* getAnimation(int action);
    int getX();
    int getY();
    int getW();
    int getH();
    bool isCameraFocused();
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

Animation* GameObject::getAnimation(int action)
{
    for(vector<Animation*>::iterator it = listAnimations.begin(); it != listAnimations.end(); ++it) {
        Animation* obj = *it;
        if(obj && obj->action == action)
        {
            return obj;
        }
    }
    return NULL;
}

bool GameObject::isCameraFocused()
{
    return cameraFocused;
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
            case SDLK_UP: yVel -= h / MOVING_SPEED; currentAction = ACTION_UP; break;
            case SDLK_DOWN: yVel += h / MOVING_SPEED; currentAction = ACTION_DOWN; break;
            case SDLK_LEFT: xVel -= w / MOVING_SPEED; currentAction = ACTION_LEFT; break;
            case SDLK_RIGHT: xVel += w / MOVING_SPEED; currentAction = ACTION_RIGHT; break;
        }
    }
}

void GameObject::move(vector<GameObject*> list)
{
    int oX = x;
    int oY = y;
    
    x += xVel;
    
    if( ( x < 0 ) || ( x + w > LEVEL_WIDTH ) )
    {
        //move back
        x -= xVel;
    }
    
    //Move the dot up or down
    y += yVel;
    
    //If the dot went too far up or down
    if( ( y < 0 ) || ( y + h > LEVEL_HEIGHT ) )
    {
        //move back
        y -= yVel;
    }
    
    // tests the collision against other objects in the screen
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
    
    // here we will show the animation depending on the
    // action of the input
    Animation* animation = getAnimation(currentAction);
    
    if(animation)
    {
        if(animation->currentFrame >= animation->numberOfFrames)
        {
            animation->currentFrame = 0;
        }
        else
        {
            animation->currentFrame++;
        }
        
        // if this is true we focus the camera
        // on the game object
        if(cameraFocused)
        {
            if(animation->numberOfFrames > 0)
            {
                apply_surface( x - camera.x, y - camera.y, animation->image
                              ,screen,animation->listClips[animation->currentFrame]);
            }
        }
        else
        {
            if(animation->numberOfFrames > 0)
            {
                apply_surface( x, y, animation->image , screen,animation->listClips[animation->currentFrame]);
            }
            else
            {
                apply_surface( x, y, animation->image , screen,NULL);
            }   
        }
    }
}

void GameObject::setCamera()
{
    //Center the camera over the dot
    camera.x = ( x + this->w / 2 ) - SCREEN_WIDTH / 2;
    camera.y = ( y + this->h / 2 ) - SCREEN_HEIGHT / 2;
    
    //Keep the camera in bounds.
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > LEVEL_WIDTH - camera.w )
    {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if( camera.y > LEVEL_HEIGHT - camera.h )
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

/////////////////////////////////////////////////////////////////////////////

const string GRANT_UP = "/Users/jonataschagas/Documents/images/grant_up.jpg";
const string GRANT_DOWN = "/Users/jonataschagas/Documents/images/grant_down.jpg";
const string GRANT_LEFT = "/Users/jonataschagas/Documents/images/grant_left.jpg";
const string GRANT_RIGHT = "/Users/jonataschagas/Documents/images/grant_right.jpg";
class Grant: public GameObject
{
    public:
        Grant();
};

Grant::Grant()
{
    x = 0, y = 0, w = 40, h = 45, offset = 0,velocity = 0,xVel = 0, yVel = 0, currentAction = ACTION_RIGHT;
    cameraFocused = true;
    
    ColorKeyRGB colorKeyGrant;
    colorKeyGrant.r = 0;
    colorKeyGrant.r = 0;
    colorKeyGrant.r = 0;
    
    Animation* animationUp = new Animation;
    animationUp->image = load_image(GRANT_UP,colorKeyGrant);
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
    animationDown->image = load_image(GRANT_DOWN,colorKeyGrant);
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
    animationLeft->image = load_image(GRANT_LEFT,colorKeyGrant);
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
    animationRight->image = load_image(GRANT_RIGHT,colorKeyGrant);
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
    x = 400, y = 50, w = 50, h = 400, offset = 0,velocity = 0,xVel = 0, yVel = 0, currentAction = 0;
    
    ColorKeyRGB colorKeyWall;
    colorKeyWall.r = 0;
    colorKeyWall.r = 0;
    colorKeyWall.r = 0;
    
    Animation* animationDull = new Animation;
    animationDull->image = load_image(WALL_MESH,colorKeyWall);
    animationDull->currentFrame = 0;
    animationDull->action = 0;
    animationDull->numberOfFrames = 0;
    listAnimations.push_back(animationDull);
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
    SDL_Surface *background = load_image(BACKGROUND_IMG,colorKeyScreen);
    SDL_Event event;
    bool quit = false;
    Timer fps;
    Grant grant;
    //Wall wall;
    vector<GameObject*> listGameObjects(2);
    listGameObjects.push_back(&grant);
    //listGameObjects.push_back(&wall);
    
    if(init(screen) == false)
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
                    obj->handle_input(&event);
                }
            }
        }
        
        fill_colorkey(screen);
        
        //Show the background
        apply_surface( 0, 0, background, screen, &camera );
        
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

