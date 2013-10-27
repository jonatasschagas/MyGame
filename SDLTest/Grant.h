//
//  Grant.h
//  SDLTest
//
//  Created by Jonatas Chagas on 26/10/2013.
//  Copyright (c) 2013 Jonatas Chagas. All rights reserved.
//

#ifndef __SDLTest__Grant__
#define __SDLTest__Grant__

#include "GameObject.h"
#include <iostream>

const string GRANT_UP = "/Users/jonataschagas/Documents/images/grant_up.jpg";
const string GRANT_DOWN = "/Users/jonataschagas/Documents/images/grant_down.jpg";
const string GRANT_LEFT = "/Users/jonataschagas/Documents/images/grant_left.jpg";
const string GRANT_RIGHT = "/Users/jonataschagas/Documents/images/grant_right.jpg";
const string GRANT_RIGHT_SHOOT = "/Users/jonataschagas/Documents/images/grant_right_shoot.jpg";
const string GRANT_UP_SHOOT = "/Users/jonataschagas/Documents/images/grant_up_shoot.jpg";
const string GRANT_LEFT_SHOOT = "/Users/jonataschagas/Documents/images/grant_left_shoot.jpg";
const string GRANT_DOWN_SHOOT = "/Users/jonataschagas/Documents/images/grant_down_shoot.jpg";

class Grant: public GameObject
{
public:
    Grant();
};


#endif /* defined(__SDLTest__Grant__) */
