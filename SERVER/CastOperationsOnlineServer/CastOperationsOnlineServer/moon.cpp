// Cast Operations Online Server
// moon.cpp 
// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly

#include "moon.h"

//=============================================================================
// default constructor
//=============================================================================
Moon::Moon() : Entity()
{
    active = false;                                 // moon firsts inactive
    spriteData.width        = moonNS::WIDTH;     // size of 1 image
    spriteData.height       = moonNS::HEIGHT;
    spriteData.rect.bottom  = moonNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right   = moonNS::WIDTH;
    cols            = moonNS::TEXTURE_COLS;
    frameDelay       = moonNS::ANIMATION_DELAY_SECONDS;
    firstFrame      = moonNS::START_FRAME;       // first frame of ship animation
    lastFrame        = moonNS::END_FRAME;         // last frame of ship animation
    currentFrame    = firstFrame;
    radius          = moonNS::COLLISION_RADIUS;  // for circular collision
    visible         = false;
    fireTimer_       = 0.0f;
    fired_           = false;
    mass = moonNS::MASS;
    collisionType = entityNS::CIRCLE;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Moon::update(float frameTime)
{
    fireTimer_ -= frameTime;                     // time remaining until fire enabled

    if (visible == false)
        return;

    if(fireTimer_ < 0)                           // if ready to fire
    {
        visible = false;                        // old moon off
        active = false;
    }

    Image::update(frameTime);

    spriteData.x += frameTime * velocity.x;     // move along X 
    spriteData.y += frameTime * velocity.y;     // move along Y

    // Wrap around screen edge
    if (spriteData.x > GAME_WIDTH)              // if off right screen edge
        //spriteData.x = -moonNS::WIDTH;       // position off left screen edge
		setActive(false);
    else if (spriteData.x < -moonNS::WIDTH)  // else if off left screen edge
        //spriteData.x = GAME_WIDTH;              // position off right screen edge
		setActive(false);
    if (spriteData.y > GAME_HEIGHT)             // if off bottom screen edge
        //spriteData.y = -moonNS::HEIGHT;      // position off top screen edge
		setActive(false);
    else if (spriteData.y < -moonNS::HEIGHT) // else if off top screen edge
        //spriteData.y = GAME_HEIGHT;             // position off bottom screen edge
		setActive(false);
}

//=============================================================================
// fire
// Fires a moon from ship
//=============================================================================
void Moon::fire(Entity *ship)
{
    if(fireTimer_ <= 0.0f)                       // if ready to fire
    {
        velocity.x = (float)cos(ship->getRadians()) * moonNS::SPEED;
        velocity.y = (float)sin(ship->getRadians()) * moonNS::SPEED;
		spriteData.x = ship->getCenterX();// - spriteData.width/2;
		spriteData.y = ship->getCenterY() -spriteData.height / 4;
        visible = true;                         // make moon visible
        active = true;                          // enable collisions
        fireTimer_ = moonNS::FIRE_DELAY_SECONDS;      // delay firing
        fired_ = true;
    }
}

//=============================================================================
// crash
// crash into planet
//=============================================================================
void Moon::crash()
{
    visible = false;
    active = false;
}

//=============================================================================
// Return Moon state in MoonStc
//=============================================================================
MoonStc Moon::getNetData()
{
    MoonStc data;

    data.X = getX();
    data.Y = getY();
    data.velocity = getVelocity();
    data.active = active;
    return data;
}
