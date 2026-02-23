// Cast Operations Online Server
// caster.cpp
#include "caster.h"

//=============================================================================
// default constructor
//=============================================================================
Caster::Caster() : Entity()
{
    spriteData.width = casterNS::WIDTH;           // size of Caster1
    spriteData.height = casterNS::HEIGHT;
    spriteData.x = 0;                   // location on screen
    spriteData.y = 0;
    spriteData.rect.bottom = casterNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = casterNS::WIDTH;
    oldX = spriteData.x;
    oldY = spriteData.y;
    oldAngle = 0.0f;
    rotation = 0.0f;
    velocity.x = 0;
    velocity.y = 0;
    frameDelay = casterNS::CASTER_ANIMATION_DELAY_SECONDS;
    firstFrame = casterNS::CASTER_FIRST_FRAME;     // first frame of caster animation
    lastFrame     = casterNS::CASTER_LAST_FRAME;     // last frame of caster animation
    currentFrame = firstFrame;
    radius = casterNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
    shieldOn = false;
    loseOn = false;
    mass = casterNS::MASS;
    strcpy_s(netIP,"000.000.000.000");  // IP address as dotted quad; nnn.nnn.nnn.nnn
    timeout = 0;
    connected = false;
    buttons = 0;
    playerN = 0;
    commWarnings = 0;
    commErrors = 0;
	charged = 0;
	emotion = EMOTION_NORMAL;
	hasCard_ = true;
}

//=============================================================================
// Initialize the Caster.
// Post: returns true if successful, false if failed
//=============================================================================
bool Caster::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
    shield.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
    shield.setFrames(casterNS::SHIELD_FIRST_FRAME, casterNS::SHIELD_LAST_FRAME);
    shield.setCurrentFrame(casterNS::SHIELD_FIRST_FRAME);
    shield.setFrameDelay(casterNS::SHIELD_ANIMATION_DELAY_SECONDS);
    shield.setLoop(false);                  // do not loop animation
    lose.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
    lose.setFrames(casterNS::LOSE_FIRST_FRAME, casterNS::LOSE_LAST_FRAME);
    lose.setCurrentFrame(casterNS::LOSE_FIRST_FRAME);
    lose.setFrameDelay(casterNS::LOSE_ANIMATION_DELAY_SECONDS);
    lose.setLoop(false);               // do not loop animation
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// draw the caster
//=============================================================================
void Caster::draw()
{
    if(loseOn)
        lose.draw(spriteData); // draw lose using current spriteData
    else
    {
        Image::draw();              // draw caster
        if(shieldOn)
            // draw shield using colorFilter 25% alpha
            shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
    }
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Caster::update(float frameTime)
{
	initializeStates();
    if(loseOn)
    {
        lose.update(frameTime);
        if(lose.getAnimationComplete())    // if lose animation complete
        {
            loseOn = false;                // turn off lose
            visible = false;
            lose.setAnimationComplete(false);
            //lose.setCurrentFrame(casterNS::LOSE_FIRST_FRAME);
        }
    }

	if (safetyTimeAfterDamage > 0) {
		safetyTimeAfterDamage -= frameTime;
		setAnimation(CASTER_REST);
		updateEmotion(EMOTION_TEARING);
		if (safetyTimeAfterDamage <= 1.0f) {
			setAnimation(CASTER_NORMAL);
		}
	}
	// blast backwards and tears when she is damaged
	/*
	if (blastBackwardsTimer > 0) {
		blastBackwardsTimer -= frameTime;
		setAnimation(REST);
		updateEmotion(EMOTION_TEARING);
	}
	*/

    if(shieldOn)
    {
        shield.update(frameTime);
		//shieldTime -= frameTime;

		/*
		if(shield.getAnimationComplete())
		{
			shieldOn = false;
			shield.setAnimationComplete(false);
		}
		*/
	}

	Entity::update(frameTime);
	oldX = spriteData.x;                        // save current position
	oldY = spriteData.y;
	oldAngle = spriteData.angle;

	spriteData.angle += frameTime * rotation;   // apply rotation

	spriteData.x += frameTime * velocity.x;     // move caster along X 
	spriteData.y += frameTime * velocity.y;     // move caster along Y

	if (spriteData.x > BOARD_X_MAX - 32)              // if off right screen edge
		spriteData.x = BOARD_X_MAX - 32;
	if (spriteData.x < BOARD_X)              // if off right screen edge
		spriteData.x = BOARD_X;
	if (spriteData.y > BOARD_Y_MAX)              // if off right screen edge
		spriteData.y = BOARD_Y_MAX;
	if (spriteData.y < BOARD_Y + 16)              // if off right screen edge
		spriteData.y = BOARD_Y + 16;

}

// virtual method
void Caster::spellCard()
{
}

//=============================================================================
// damage
//=============================================================================
void Caster::damage(WEAPON weapon)
{
	if (safetyTimeAfterDamage > 0)
		return;
	//if (shieldOn)
	if (shieldTime > 0)
		return;

	switch (weapon)
	{
	case BUGBALL:
		//audio->playCue(BUGBALL_HIT);
		health -= casterNS::BUGBALL_DAMAGE;
		setSafetyTimeAfterDamage(2.5f);
		break;
	case MOON:
		health -= casterNS::MOON_DAMAGE;
		setSafetyTimeAfterDamage(2.5f);
		break;
	}
	if (health <= 0)
		explode();
}

//=============================================================================
// explode
//=============================================================================
void Caster::explode()
{
	active = false;
	health = 0;
	loseOn = true;
	shieldOn = false;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

//=============================================================================
// repair
//=============================================================================
void Caster::repair()
{
	active = true;
	health = FULL_HEALTH;
	loseOn = false;
	shieldOn = false;
	rotation = 0.0f;
	visible = true;
}


//=============================================================================
// Return current caster state in CasterStc
//=============================================================================
CasterStc Caster::getNetData()
{
	CasterStc data;
	data.X = getX();
	data.Y = getY();
	data.radians = getRadians();
	data.health = getHealth();
	data.velocity = getVelocity();
	data.rotation = getRotation();
	data.charged = getCharged();
	data.playerN = getPlayerN();
	data.anim = getAnimation();
	data.emotion = getEmotion();
	//-----flags-----
	// bit0 active
	// bit2 shieldOn
	data.flags = 0;
	if (getActive())
		data.flags |= 0x01;
	if (getShieldOn())
		data.flags |= 0x04;
	return data;
}
