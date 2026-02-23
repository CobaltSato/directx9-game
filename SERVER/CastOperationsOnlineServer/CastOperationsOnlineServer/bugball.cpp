// Cast Operations Online Server
// bugball.cpp 

#include "bugball.h"

//=============================================================================
// default constructor
//=============================================================================
Bugball::Bugball() : Entity()
{
    active = false;                                 // bugball firsts inactive
    spriteData.width        = bugballNS::WIDTH;     // size of 1 image
    spriteData.height       = bugballNS::HEIGHT;
    spriteData.rect.bottom  = bugballNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right   = bugballNS::WIDTH;
    cols            = bugballNS::TEXTURE_COLS;
    frameDelay       = bugballNS::ANIMATION_DELAY_SECONDS;
    firstFrame      = bugballNS::FIRST_FRAME;       // first frame of ship animation
    lastFrame        = bugballNS::LAST_FRAME;         // last frame of ship animation
    currentFrame    = firstFrame;
    radius          = bugballNS::COLLISION_RADIUS;  // for circular collision
    visible         = false;
    fireTimer_       = 0.0f;
    fired_           = false;
    mass = bugballNS::MASS;
    collisionType = entityNS::CIRCLE;
	setScale(0.7f);
	thinkTimer_ = 3;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Bugball::update(float frameTime)
{
    fireTimer_ -= frameTime;                     // time remaining until fire enabled

    if (visible == false)
        return;

    if(fireTimer_ < 0)                           // if ready to fire
    {
        visible = false;                        // old bugball off
        active = false;
    }

    Image::update(frameTime);

    spriteData.x += frameTime * velocity.x;     // move along X 
    spriteData.y += frameTime * velocity.y;     // move along Y

    // Wrap around screen edge
    if (spriteData.x > GAME_WIDTH)              // if off right screen edge
        spriteData.x = -bugballNS::WIDTH;       // position off left screen edge
    else if (spriteData.x < -bugballNS::WIDTH)  // else if off left screen edge
        spriteData.x = GAME_WIDTH;              // position off right screen edge
    if (spriteData.y > GAME_HEIGHT)             // if off bottom screen edge
        spriteData.y = -bugballNS::HEIGHT;      // position off top screen edge
    else if (spriteData.y < -bugballNS::HEIGHT) // else if off top screen edge
        spriteData.y = GAME_HEIGHT;             // position off bottom screen edge
}

//=============================================================================
// fire
// Fires a bugball from ship
//=============================================================================
void Bugball::fire(Entity *ship)
{
	setThinkTimer(0.0f);
    if(fireTimer_ <= 0.0f)                       // if ready to fire
    {
        velocity.x = (float)cos(ship->getRadians()) * bugballNS::SPEED;
        velocity.y = (float)sin(ship->getRadians()) * bugballNS::SPEED;
        spriteData.x = ship->getCenterX() - spriteData.width/2;
        spriteData.y = ship->getCenterY() - spriteData.height/2;
        visible = true;                         // make bugball visible
        active = true;                          // enable collisions
        fireTimer_ = bugballNS::FIRE_DELAY_SECONDS;      // delay firing
        fired_ = true;
    }
}

//=============================================================================
// crash
// crash into planet
//=============================================================================
void Bugball::crash()
{
    visible = false;
    active = false;
}

//=============================================================================
// Return Bugball state in BugballStc
//=============================================================================
BugballStc Bugball::getNetData()
{
    BugballStc data;

    data.X = getX();
    data.Y = getY();
    data.velocity = getVelocity();
    data.active = active;
    return data;
}
