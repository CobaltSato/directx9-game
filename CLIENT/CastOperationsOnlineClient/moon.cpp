
// Cast Operations Online Client
// moon.cpp

#include "moon.h"

//=============================================================================
// default constructor
//=============================================================================
Moon::Moon() : Entity()
{
    active = false;                                 // moon starts inactive
    spriteData.width        = moonNS::WIDTH;     // size of 1 image
    spriteData.height       = moonNS::HEIGHT;
    spriteData.rect.bottom  = moonNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right   = moonNS::WIDTH;
    cols            = moonNS::TEXTURE_COLS;
    frameDelay      = moonNS::ANIMATION_DELAY_SECONDS;
    firstFrame      = moonNS::FIRST_FRAME;       // first frame of ship animation
    lastFrame        = moonNS::LAST_FRAME;         // last frame of ship animation
    currentFrame    = firstFrame;
    radius          = moonNS::COLLISION_RADIUS;  // for circular collision
    visible         = false;
    mass = moonNS::MASS;
	setScale(0.5f);
    collisionType = entityNS::CIRCLE;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Moon::update(float frameTime)
{
    if (visible == false)
        return;

    Image::update(frameTime);

    spriteData.x += frameTime * velocity.x;             // move along X 
    spriteData.y += frameTime * velocity.y;             // move along Y

	setRadians(atan2(getVelocity().y,getVelocity().x));

}
//=============================================================================
// crash
// crash into planet
//=============================================================================
void Moon::crash()
{
    visible = false;
    active = false;
    audio->playCue(BUGBALL_CRASH);
}

//=============================================================================
// Sets all moon data from MoonStc sent from server to client
//=============================================================================
void Moon::setNetData(MoonStc ts)
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
