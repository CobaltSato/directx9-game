// Cast Operations Online Client
// caster.h

#ifndef _CASTER_H                 // Prevent multiple definitions if this 
#define _CASTER_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "entity.h"
#include "constants.h"

namespace casterNS
{
	const int   WIDTH = 32;                 // image width (each frame)
	const int   HEIGHT = 32;                // image height
	const int   TEXTURE_COLS = 3;           // texture has 8 columns
	const float SPEED = 100;                // 100 pixels per second
	const float MASS = 17.0f;              // mass

	const int   LOSE_FIRST_FRAME = 33; // lose start frame
	const int   LOSE_LAST_FRAME = 35;   // lose end frame
	const float LOSE_ANIMATION_DELAY_SECONDS = 0.6f;   // time between frames

	const int   SHIELD_FIRST_FRAME = 24;    // shield start frame
	const int   SHIELD_LAST_FRAME = 27;      // shield end frame
	const float SHIELD_ANIMATION_DELAY = 0.1f; // time between frames
	const int SHIELD_WIDTH = 32; // time between frames
	const int SHIELD_HEIGHT = 32; // time between frames
	const int SHIELD_COLS = 8; // time between frames

	const int   CASTER_FIRST_FRAME = 0;     
	const int   CASTER_LAST_FRAME = 47;        
	const int	CASTER_DOWN_FIRST = 0;
	const int	CASTER_DOWN_LAST = 2;
	const int	CASTER_LEFT_FIRST = 3;
	const int	CASTER_LEFT_LAST = 5;
	const int	CASTER_RIGHT_FIRST = 6;
	const int	CASTER_RIGHT_LAST = 8;
	const int	CASTER_UP_FIRST = 9;
	const int	CASTER_UP_LAST = 11;
	const int	CASTER_NORMAL_FIRST = 12;
	const int	CASTER_NORMAL_LAST = 14;
	const int	CASTER_REST_FIRST = 18;
	const int	CASTER_REST_LAST = 20;
	const int	CASTER_SLEEP_FIRST = 33;
	const int	CASTER_SLEEP_LAST = 35;

	const float CASTER_ANIMATION_DELAY_SECONDS = 0.4f;    // time between frames

}

// CasterStc contains all of the information a network client needs for a caster.
struct CasterStc
{
	float X;
	float Y;
	float radians;
	float health;
	VECTOR2 velocity;
	float rotation;             // rotation rate (radians/second)
	float charged;
	UCHAR playerN;              // which player (255 is request to join)
	CASTER_ANIMATION anim;
	EMOTION emotion;
	//-----flags-----
	//=====================================================================
	// bit0 active              // true when player is active
	// bit1 engineOn
	// bit2 shieldOn
	UCHAR flags;                // boolean status flags
};

// inherits from Entity class
class Caster : public Entity
{
private:
	DISALLOW_COPY_AND_ASSIGN(Caster);
	float   oldX, oldY, oldAngle;
	float   rotation;               // current rotation rate (radians/second)
	float   loseTimer;
	int     score;
	bool    loseOn;
	Image   lose;
	CASTER_ANIMATION anim; // dot animations
	float charged; // how much charged (in percent)

	// Network Variables
	char    netIP[16];      // IP address as dotted quad; nnn.nnn.nnn.nnn
	int     timeout;
	bool    connected;
	UCHAR   buttons;        // current key presses
							// bit 0=Left, 1=Forward, 2=Right, 3=Fire
	u_long  packetNumber;   // used to detect out of order packets
	UCHAR   playerN;        // our player number
	int     commWarnings;   // count of communication warnings
	int     commErrors;     // count of communication errors

protected:
	Image	face;
	Image   shield;
	bool    shieldOn;
	float  shieldTime;
	EMOTION  emotion;
	bool hasCard_; // true if she has not used the spellCard() yet

public:
	// constructor
	Caster();

	// virtual function for the personality of a caster
	virtual void spellCard() = 0;

	// inherited member functions
	virtual void draw();

	// initialize the caster class for face, barriar, dot textures
	bool initialize(
		Game *gamePtr,
		TextureManager *textureM,
		ImageStc imageStc,
		TextureManager *faceTextureM, // additional face Texture
		ImageStc faceStc,
		TextureManager *gameTextureM // additional face Texture
	);

	// update caster position and angle
	void update(float frameTime);

	// new member functions

	// move caster out of collision
	void toOldPosition()
	{
		spriteData.x = oldX;
		spriteData.y = oldY,
			spriteData.angle = oldAngle;
		rotation = 0.0f;
	}

	// Returns rotation
	float getRotation() { return rotation; }

	// Returns shieldOn condition
	bool getShieldOn() { return shieldOn; }

	// Return score
	int getScore() { return score; }

	// Return IP address of this player as dotted quad; nnn.nnn.nnn.nnn
	const char* getNetIP() { return netIP; }

	// Return netTimeout count
	int getTimeout() { return timeout; }

	// Return netConnected boolean
	bool getConnected() { return connected; }

	// Return buttons
	UCHAR getButtons() { return buttons; }

	// Return packetNumber
	u_long getPacketNumber() { return packetNumber; }

	// Return Caster Data
	//CasterStc getNetData();

	// Return player number
	int getPlayerN() { return playerN; }

	// Return commWarnings
	int getCommWarnings() { return commWarnings; }

	// Return commErrors
	int getCommErrors() { return commWarnings; }

	// Return loseOn
	bool getLoseOn() { return loseOn; }

	// Set shield on
	void setShieldOn(bool sh) { shieldOn = sh; }

	// Sets Mass
	void setMass(float m) { mass = m; }

	// Set rotation rate
	void setRotation(float r) { rotation = r; }

	// Sets all caster data from CasterStc sent from server to client
	void setNetData(CasterStc ss);

	// Set IP address of this player as dotted quad; nnn.nnn.nnn.nnn
	void setNetIP(const char* address) { strcpy_s(netIP, address); }

	// Set netConnected boolean
	void setConnected(bool c) { connected = c; }

	// Set buttons
	void setButtons(UCHAR b) { buttons = b; }

	// Set packetNumber
	void setPacketNumber(u_long n) { packetNumber = n; }

	// Set commWarnings
	void setCommWarnings(int w) { commWarnings = w; }

	// Set commErrors
	void setCommErrors(int e) { commErrors = e; }

	// Set timeout
	void setTimeout(int t) { timeout = t; }

	// Increment commWarnings
	void incCommWarnings() { commWarnings++; }

	// Increment commErrors
	void incCommErrors() { commErrors++; }

	// Increment netTimeout count
	void incTimeout() { timeout++; }

	// caster explodes
	void explode();

	// caster is repaired
	void repair();

	//  getters, setters, and other trivial member functions
	//-----------------------------------------------------------
	void setAnimation(CASTER_ANIMATION a) {
		anim = a;
	}
	CASTER_ANIMATION getAnimation() {
		return anim;
	}
	void setCharged(float c) {
		charged = c;
	}
	float getCharged() {
		return charged;
	}

	// emotional functions
	void updateEmotion(EMOTION e) {
		// a stronger emotion is attached to 
		// larger number than a weaker emotion
		if(emotion < e)
			emotion = e;
	}
	void setEmotion(EMOTION e) {
		emotion = e;
	}
	EMOTION getEmotion() {
		return emotion;
	}
	void initializeEmotion() {
		emotion = EMOTION_NORMAL; // 0 -- neutral emotion 
	}
	// reflect an emotion by mapping an emotion to the corresponding frame
	void reflectEmotion() {
		face.setCurrentFrame(EMOTION_TO_FRAME[emotion]);
	}
	float getShieldTime() {
		return shieldTime;
	}



};
#endif

