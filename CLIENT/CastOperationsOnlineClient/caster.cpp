// Cast Operations Online Client
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
	shieldTime = 0;
    frameDelay = casterNS::CASTER_ANIMATION_DELAY_SECONDS ;
	hasCard_ = true;
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
	health = FULL_HEALTH;
	emotion = EMOTION_NORMAL;
}

//=============================================================================
// Initialize the Caster.
// Post: returns true if successful, false if failed
//=============================================================================
bool Caster::initialize(
	Game *gamePtr,
	TextureManager *textureM,
	ImageStc imageStc,
	TextureManager *faceTextureM, // additional face Texture
	ImageStc faceStc,
	TextureManager *gameTextureM // additional face Texture
) {

    shield.initialize(gamePtr->getGraphics(), casterNS::SHIELD_WIDTH,casterNS::SHIELD_HEIGHT,casterNS::SHIELD_COLS, gameTextureM);
    shield.setFrames(casterNS::SHIELD_FIRST_FRAME, casterNS::SHIELD_LAST_FRAME);
    shield.setCurrentFrame(casterNS::SHIELD_FIRST_FRAME);
    shield.setFrameDelay(casterNS::SHIELD_ANIMATION_DELAY);
    shield.setLoop(true);                  // do not loop animation

    lose.initialize(gamePtr->getGraphics(), imageStc.WIDTH, imageStc.HEIGHT, imageStc.NCOLS, textureM);
    lose.setFrames(casterNS::LOSE_FIRST_FRAME, casterNS::LOSE_LAST_FRAME);
    lose.setCurrentFrame(casterNS::LOSE_FIRST_FRAME);
    lose.setFrameDelay(casterNS::LOSE_ANIMATION_DELAY_SECONDS);
    lose.setLoop(false);               // do not loop animation
	bool faceInitialized = 
	face.initialize(
		gamePtr->getGraphics(),
		faceStc.WIDTH,
		faceStc.HEIGHT,
		faceStc.NCOLS,
		faceTextureM
	);

	if (faceInitialized == true) {
		face.setX(faceStc.X);
		face.setY(faceStc.Y);
		face.setCurrentFrame(faceStc.FRAME_FIRST);
		face.setFrames(faceStc.FRAME_FIRST, faceStc.FRAME_LAST);
	}

	bool entityInitialized = 
    (Entity::initialize(
		gamePtr,
		imageStc.WIDTH,
		imageStc.HEIGHT,
		imageStc.NCOLS,
		textureM));

	if (entityInitialized == true) {
		setX(imageStc.X);
		setY(imageStc.Y);
		//setCurrentFrame(imageStc.FRAME_LAST);
		//setFrames(imageStc.FRAME_LAST, imageStc.FRAME_END);
		setVelocity(VECTOR2(0, 0));
		setMass(casterNS::MASS);
		firstFrame = imageStc.FRAME_FIRST;     // first frame of caster animation
		lastFrame = imageStc.FRAME_LAST;     // last frame of caster animation
		currentFrame = firstFrame;
	}

	return faceInitialized && entityInitialized;
}

//=============================================================================
// draw the caster
//=============================================================================
void Caster::draw()
{
	reflectEmotion();
	face.draw();
	if (loseOn)
		lose.draw(spriteData); // draw explosion using current spriteData
	else
	{
		Image::draw();              // draw caster
		if (shieldOn)
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
	// lose
	if (loseOn)
	{
		lose.update(frameTime);
		if (lose.getAnimationComplete())    // if explosion animation complete
		{
			loseOn = false;                // turn off explosion
			visible = false;
			lose.setAnimationComplete(false);
			lose.setCurrentFrame(casterNS::LOSE_LAST_FRAME);
		}
	}

	// shield
	if (shieldOn)
	{
		shield.update(frameTime);
	}

	Entity::update(frameTime);

	// save current position
	oldX = spriteData.x;                      
	oldY = spriteData.y;
	oldAngle = spriteData.angle;

	spriteData.x += frameTime * velocity.x;     // move caster along X 
	spriteData.y += frameTime * velocity.y;     // move caster along Y

	// let casters inside a tile stage
	if (spriteData.x > GAME_WIDTH)              // if off right screen edge
		spriteData.x = GAME_WIDTH;
	if (spriteData.x < 0)              // if off right screen edge
		spriteData.x = 0;
	if (spriteData.y > GAME_HEIGHT)              // if off right screen edge
		spriteData.y = GAME_HEIGHT;
	if (spriteData.y < 0)              // if off right screen edge
		spriteData.y = 0;

	// switch the animation by the state data received from the server
	//===============================================================================
	switch (getAnimation()) {
	case(CASTER_NORMAL):
		//setCurrentFrame(casterNS::CASTER_NORMAL);
		setFrames(casterNS::CASTER_NORMAL_FIRST, casterNS::CASTER_NORMAL_LAST);
		break;
	case(CASTER_UP):
		//setCurrentFrame(casterNS::CASTER_UP);
		setFrames(casterNS::CASTER_UP_FIRST, casterNS::CASTER_UP_LAST);
		break;
	case(CASTER_RIGHT):
		//setCurrentFrame(casterNS::CASTER_RIGHT);
		setFrames(casterNS::CASTER_RIGHT_FIRST, casterNS::CASTER_RIGHT_LAST);
		break;
	case(CASTER_DOWN):
		//setCurrentFrame(casterNS::CASTER_DOWN);
		setFrames(casterNS::CASTER_DOWN_FIRST, casterNS::CASTER_DOWN_LAST);
		break;
	case(CASTER_LEFT):
		//setCurrentFrame(casterNS::CASTER_LEFT);
		setFrames(casterNS::CASTER_LEFT_FIRST, casterNS::CASTER_LEFT_LAST);
		break;
	case(CASTER_REST):
		//setCurrentFrame(casterNS::CASTER_REST_FIRST);
		setFrames(casterNS::CASTER_REST_FIRST, casterNS::CASTER_REST_LAST);
		break;
	}

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
// Sets all caster data from CasterStc sent from server to client
//=============================================================================
void Caster::setNetData(CasterStc ss)
{
	setX(ss.X);
	setY(ss.Y);
	setRadians(ss.radians);
	setHealth(ss.health);
	setVelocity(ss.velocity);
	setRotation(ss.rotation);
	setActive((ss.flags & 0x01) == 0x01);
	if (active)
		visible = true;
	setShieldOn((ss.flags & 0x04) == 0x04);
	setCharged(ss.charged);
	setEmotion(ss.emotion);
	setAnimation(ss.anim);
	if (health <= 0 && loseOn == false && visible) // if caster destroyed
		explode();
}

