#pragma once
// Cast Operations Online Server
// moon.h
// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly

#ifndef _MOON_H              // Prevent multiple definitions if this 
#define _MOON_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace moonNS
{
    const int   WIDTH = 64;             // image width
    const int   HEIGHT = 64 ;           // image height
	const int   COLLISION_RADIUS = 16;   // for circular collision
    const float SPEED = 200;            // pixels per second
    const float MASS = 300.0f;          // mass
    const float FIRE_DELAY_SECONDS = 1.0f;      // 4 seconds between moon firing
    const int   TEXTURE_COLS = 1;       // texture has 8 columns
    const int   START_FRAME = 0;       // starts at frame 40
    const int   END_FRAME = 0;         // animation frames 40,41,42,43
    const float ANIMATION_DELAY_SECONDS = 0.1f; // time between frames
}

// MoonStc contains all of the information a client needs for one moon.
struct MoonStc 
{
    float X;
    float Y;
    VECTOR2 velocity;
    bool  active;                       // true when active
};

// inherits from Entity class
class Moon : public Entity
{
private:
	DISALLOW_COPY_AND_ASSIGN(Moon);
    float   fireTimer_;                  // time remaining until fire enabled
    bool    fired_;
public:
    Moon();

    // inherited member functions
    void update(float frameTime);
    float getMass() const   {return moonNS::MASS;}

    // Return fired_ bool
    bool getFired() const   {return fired_;}

    // Return moon data
    MoonStc getNetData();

    // Set fired_ bool
    void setFired(bool f)   {fired_ = f;}

    // new member functions
    void fire(Entity *ship);                // fire moon from ship
    void crash();                           // crash into planet

	// velocity setter 
	void setVelocity(VECTOR2 v) {
		velocity.x = v.x;
		velocity.y = v.y;
	}

    // Sets all moon data from MoonStc sent from server to client
    //void setNetData(MoonStc ts);
};
#endif

