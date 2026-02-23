// Cast Operations Online Client
// bugball.h

#ifndef _BUGBALL_H              // Prevent multiple definitions if this 
#define _BUGBALL_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace bugballNS
{
    const int   WIDTH = 64;             // image width
    const int   HEIGHT = 64 ;           // image height
    const int   COLLISION_RADIUS = 4;   // for circular collision
    const float ROTATION_RATE = 2.0f*(float)PI; // 2 PI radians per second (360 degrees)
    const float SPEED = 200;            // pixels per second
    const float MASS = 300.0f;          // mass
    const float FIRE_DELAY = 2.0f;      // 4 seconds between bugball firing
    const int   TEXTURE_COLS = 3;       // texture has 8 columns
    const int   FIRST_FRAME = 0;       // starts at frame 40
    const int   LAST_FRAME = 2;         // animation frames 40,41,42,43
    const float ANIMATION_DELAY = 0.1f; // time between frames
}

// BugballStc contains all of the information a client needs for one bugball.
struct BugballStc 
{
    float X;
    float Y;
    VECTOR2 velocity;
    bool  active;                       // true when active
};

// inherits from Entity class
class Bugball : public Entity
{
private:
	DISALLOW_COPY_AND_ASSIGN(Bugball);
    float   fireTimer_;                  // time remaining until fire enabled
public:
    // constructor
    Bugball();

    // inherited member functions
    void update(float frameTime);
    float getMass() const   {return bugballNS::MASS;}

    // new member functions
    void crash();                           // crash into planet

    // Sets all bugball data from BugballStc sent from server to client
    void setNetData(BugballStc ts);
};
#endif

