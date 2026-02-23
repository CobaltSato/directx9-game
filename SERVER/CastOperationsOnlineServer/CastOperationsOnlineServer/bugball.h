// Cast Operations Online Server
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
	const int   COLLISION_RADIUS = 16;   // for circular collision
    const float SPEED = 200;            // pixels per second
    const float MASS = 300.0f;          // mass
    const float FIRE_DELAY_SECONDS = 20.0f;      // 4 seconds between bugball firing
    const int   TEXTURE_COLS = 3;       // texture has 8 columns
    const int   FIRST_FRAME = 0;       // starts at frame 40
    const int   LAST_FRAME = 2;         // animation frames 40,41,42,43
    const float ANIMATION_DELAY_SECONDS = 0.1f; // time between frames
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
    bool    fired_;
	float	thinkTimer_; // time for a next interval when the ball decide the direction to the enemy
public:
    Bugball();
	~Bugball() {};

    // inherited member functions
    void update(float frameTime);

	// cancel the fire
	void cancel() {
		active = false;
		fireTimer_ = 0;
	}
    // new member functions
    void fire(Entity *ship);                // fire bugball from ship
    void crash();                           // crash into planet

	// network functions
	//-----------------------------------------------------------------
    // Return bugball data
    BugballStc getNetData();
    // Sets all bugball data from BugballStc sent from server to client
    // void setNetData(BugballStc ts); (client side function)

	// trivial functions
	//-----------------------------------------------------------------
    void setFired(bool f)   {fired_ = f;}

	void setVelocity(VECTOR2 v) {
		velocity.x = v.x;
		velocity.y = v.y;
	}
	float getThinkTimer()
	{
		return thinkTimer_;
	}
	void setThinkTimer(float t) {
		thinkTimer_ = t;
	}

    float getMass() const   {return bugballNS::MASS;}

    bool getFired() const   {return fired_;}
};
#endif

