// Cast Operations Online Client
// moon.h

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
    const float FIRE_DELAY_SECONDS = 4.0f;      // 4 seconds between moon firing
    const int   TEXTURE_COLS = 1;       // texture has 8 columns
    const int   FIRST_FRAME = 0;       // starts at frame 40
    const int   LAST_FRAME = 0;         // animation frames 40,41,42,43
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
public:
    Moon();
	~Moon() {};

    // inherited member functions
    void update(float frameTime);
    float getMass() const   {return moonNS::MASS;}

    // new member functions
    void crash();                           // crash into planet

    // Sets all moon data from MoonStc sent from server to client
    void setNetData(MoonStc ts);
};
#endif

