// Cast Operations Online Client
// bugball.cpp

#include "bugball.h"

//=============================================================================
// default constructor
//=============================================================================
Bugball::Bugball() : Entity()
{
    active = false;                                 // bugball starts inactive
    spriteData.width        = bugballNS::WIDTH;     // size of 1 image
    spriteData.height       = bugballNS::HEIGHT;
    spriteData.rect.bottom  = bugballNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right   = bugballNS::WIDTH;
    cols            = bugballNS::TEXTURE_COLS;
    frameDelay       = bugballNS::ANIMATION_DELAY;
    firstFrame      = bugballNS::FIRST_FRAME;       // first frame of ship animation
    lastFrame        = bugballNS::LAST_FRAME;         // last frame of ship animation
    currentFrame    = firstFrame;
    radius          = bugballNS::COLLISION_RADIUS;  // for circular collision
    visible         = false;
    fireTimer_       = 0.0f;
    mass = bugballNS::MASS;
    collisionType = entityNS::CIRCLE;
	setScale(0.9f);
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Bugball::update(float frameTime)
{
    //fireTimer_ -= frameTime;                     // time remaining until fire enabled

    if (visible == false)
        return;

    //if(fireTimer_ < 0)                           // if ready to fire
    //    visible = false;                        // old bugball off

    Image::update(frameTime);

    spriteData.x += frameTime * velocity.x;             // move along X 
    spriteData.y += frameTime * velocity.y;             // move along Y

    // Wrap around screen edge
    if (spriteData.x > GAME_WIDTH)                      // if off right screen edge
        spriteData.x = -bugballNS::WIDTH;               // position off left screen edge
    else if (spriteData.x < -bugballNS::WIDTH)          // else if off left screen edge
        spriteData.x = GAME_WIDTH;                      // position off right screen edge
    if (spriteData.y > GAME_HEIGHT)                     // if off bottom screen edge
        spriteData.y = -bugballNS::HEIGHT;              // position off top screen edge
    else if (spriteData.y < -bugballNS::HEIGHT)         // else if off top screen edge
        spriteData.y = GAME_HEIGHT;                     // position off bottom screen edge
}
//=============================================================================
// crash
// crash into planet
//=============================================================================
void Bugball::crash()
{
    visible = false;
    active = false;
    audio->playCue(BUGBALL_CRASH);
}

//=============================================================================
// Sets all bugball data from BugballStc sent from server to client
//=============================================================================
void Bugball::setNetData(BugballStc ts)
{
    setActive(ts.active);
    if(active)
        setVisible(true);
    else
        setVisible(false);
    setX(ts.X);
    setY(ts.Y);
    setVelocity(ts.velocity);
}
