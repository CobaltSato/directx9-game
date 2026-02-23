//******************************************************************************
// Cast Operations Network Server
// castOperations.cpp  -- the core class for the game
// Author : C.S.
//******************************************************************************

#include "castOperations.h"
#include "satori.h"
#include "koishi.h"
using namespace castOperationsNS;

//=============================================================================
// Constructor
//=============================================================================
CastOperations::CastOperations()
{
    menuOn = true;
    countDownOn = false;
    initialized = false;
    port = netNS::DEFAULT_PORT;
    netTime = 0;
    startTimerRun = false;
    startTimer = 0;
    playerCount = 0;
    menuTimer = 0;
	caster[0] = &satori;
	caster[1] = &koishi;
}

//=============================================================================
// Destructor
//=============================================================================
CastOperations::~CastOperations()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void CastOperations::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError
	initializeTextures();
	initializeObjects();
}

//=============================================================================
// Initializes the game textures
// Throws GameError on error
//=============================================================================
void CastOperations::initializeTextures() {
	// menu texture
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// nebula texture
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// main game textures
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// komeiji textures
	if (!satoriTexture.initialize(graphics, SATORI_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));
	if (!koishiTexture.initialize(graphics, KOISHI_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	//bugball textures
	if (!bugballTextures.initialize(graphics, BUGBALL_IMAGE, TRANSCOLOR_BLACK))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	//bugball textures
	if (!moonTexture.initialize(graphics, MOON_IMAGE, TRANSCOLOR_WHITE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

}

//=============================================================================
// Initializes the game objects
// Throws GameError on error
//=============================================================================
void CastOperations::initializeObjects(){
	// menu image
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // casters
    if (!satori.initialize(this, casterNS::WIDTH,casterNS::HEIGHT,casterNS::TEXTURE_COLS, &satoriTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing caster1"));
    satori.setCurrentFrame(casterNS::CASTER_NORMAL);
    satori.setMass(casterNS::MASS);
    satori.setX(satoriConstantsNS::DOT_X);
    satori.setY(satoriConstantsNS::DOT_Y);
    if (!koishi.initialize(this, casterNS::WIDTH,casterNS::HEIGHT,casterNS::TEXTURE_COLS, &koishiTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing caster2"));
    koishi.setCurrentFrame(casterNS::CASTER_NORMAL);
    koishi.setMass(casterNS::MASS);
    koishi.setX(koishiConstantsNS::DOT_X);
    koishi.setY(koishiConstantsNS::DOT_Y);

    // bugballs
    if (!bugball[0].initialize(this, bugballNS::WIDTH, bugballNS::HEIGHT, bugballNS::TEXTURE_COLS, &bugballTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bugball1"));
    if (!bugball[1].initialize(this, bugballNS::WIDTH, bugballNS::HEIGHT, bugballNS::TEXTURE_COLS, &bugballTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bugball2"));

    // moons
    if (!moon[0].initialize(this, moonNS::WIDTH, moonNS::HEIGHT, moonNS::TEXTURE_COLS, &moonTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moon1"));
    if (!moon[1].initialize(this, moonNS::WIDTH, moonNS::HEIGHT, moonNS::TEXTURE_COLS, &moonTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moon2"));


    toClientData.gameState = 0;
    toClientData.sounds = 0;
    initializeServer(port);     // initialize game server

    roundOver = true;
    return;
}

//=============================================================================
// Start a new round of play
//=============================================================================
void CastOperations::roundStart()
{
    // Start casters on opposite sides of planet in stable clockwise orbit
    caster[0]->setX(GAME_WIDTH/4 - casterNS::WIDTH);
    caster[1]->setX(GAME_WIDTH/2 - GAME_WIDTH/4);
    caster[0]->setY(GAME_HEIGHT/2 - casterNS::HEIGHT  + 60);
    caster[1]->setY(GAME_HEIGHT/2 + 60);
    caster[0]->setVelocity(VECTOR2(0,0));
    caster[1]->setVelocity(VECTOR2(0,0));

    caster[0]->setDegrees(0);
    caster[1]->setDegrees(0);
    caster[0]->repair();
    caster[1]->repair();
    countDownTimer = castOperationsNS::COUNT_DOWN;
    countDownOn = true;
    roundOver = false;
    // set the state to indicate new round
    toClientData.gameState |= ROUND_START_BIT;
}

//=============================================================================
// Update all game items
//=============================================================================
void CastOperations::update()
{
	// initialize some variables before updating
	//---------------------------------------------------------------
	int casterCount = 0;      // visible casters
	playerCount = 0;
	if (menuOn)
	{
		menuTimer += frameTime;
		if (menuTimer >= MENU_TIME)
		{
			menuOn = false;
			console->show();
		}
	}
    if(startTimerRun)
    {
        startTimer += frameTime;
        if(startTimer >= START_TIME)
        {
            startTimer = 0;
            startTimerRun = false;
            roundStart();
        }
    }

    if(countDownOn)
    {
        countDownTimer -= frameTime;
        if(countDownTimer <= 0)
        {
            countDownOn = false;
            // round start bit off
            toClientData.gameState &= (0xFF ^ ROUND_START_BIT);
        }
    } 
    else
    {
        for (int i=0; i<MAX_PLAYERS; i++)       // for all players
        {
            if(caster[i]->getConnected())
                playerCount++;  // count connected players

            if(caster[i]->getVisible())
                casterCount++;        // count visible casters

			// set false on the flag of engine 1 and 2
            //toClientData.sounds &= (0xFF ^ ENGINE1_BIT);    
            //toClientData.sounds &= (0xFF ^ ENGINE2_BIT);   

            if (caster[i]->getActive())
            {
				// cancel the input for a moment after damage
				if (caster[i]->getSafetyTimeAfterDamage() >= 1.0f)
					caster[i]->setButtons(0);

				/*
                if (caster[i]->getButtons() & FORWARD_BIT) // if move forward button
                {
                    caster[i]->setEngineOn(true);
                    if(i==0)        // if caster1
                        toClientData.sounds |= ENGINE1_BIT; // sound on
                    else            // if caster2
                        toClientData.sounds |= ENGINE2_BIT; // sound on
                }
                else
                    caster[i]->setEngineOn(false); // engine off
				*/

				if (caster[i]->getButtons() & RIGHT_BIT)    // if fire button
				{
					caster[i]->setX(caster[i]->getX() + 60*frameTime);
					caster[i]->setAnimation(CASTER_RIGHT);
					if (caster[i]->getX() > GAME_WIDTH)               // if off screen right
						caster[i]->setX((float)-caster[i]->getWidth());     // position off screen left

				}
				if (caster[i]->getButtons() & LEFT_BIT)    // if fire button
				{
					caster[i]->setX(caster[i]->getX() - 60*frameTime);
					caster[i]->setAnimation(CASTER_LEFT);
					if (caster[i]->getX() < -caster[i]->getWidth())         // if off screen left
						caster[i]->setX((float)GAME_WIDTH);           // position off screen right
				}
				if (caster[i]->getButtons() & UP_BIT)    // if fire button
				{
					caster[i]->setY(caster[i]->getY() - frameTime * 60);
					caster[i]->setAnimation(CASTER_UP);
					if (caster[i]->getY() < -caster[i]->getHeight())        // if off screen top
						caster[i]->setY((float)GAME_HEIGHT);          // position off screen bottom
				}
				if (caster[i]->getButtons() & DOWN_BIT)    // if fire button
				{
					caster[i]->setAnimation(CASTER_DOWN);
					caster[i]->setY(caster[i]->getY() + frameTime * 60);
					if (caster[i]->getY() > GAME_HEIGHT)              // if off screen bottom
						caster[i]->setY((float)-caster[i]->getHeight());    // position off screen top
				}


				if (caster[i]->getButtons() & FIRE_BIT)    // if fire button
				{
					if (moon[i].getActive() == false) {
						moon[i].fire(caster[i]);          // fire bugball
						VECTOR2 v;
						if (i == 0) {
							v.x = 1 * static_cast<float>(500);
						}
						else {
							v.x = -1 * static_cast<float>(500);
						}
						v.y = 0;
						moon[i].setVelocity(v);
					}
				}

				if (caster[i]->getButtons() & CHARGE_BIT)    // if fire button
				{
					caster[i]->charge(frameTime);
				}
				else {
					caster[i]->chargeOff();
					if (caster[i]->getCharged() >= 10) {
						bugball[i].cancel();
						caster[i]->setCharged(0);
						bugball[i].fire(caster[i]);          // fire bugball
						if (bugball[i].getFired())           // if it fired
						{
							// change the state of the sound bit to play the sound
							toClientData.sounds ^= SATORI_FIRE_BIT << (2 * i);
							bugball[i].setFired(false);     // do not play sound again
						}
					}
				}

				if (caster[i]->getButtons() & SPELL_BIT)    // if fire button
				{
					caster[i]->spellCard();
					// the caster laughs, and the enemy is surprised
					//caster[i]->updateEmotion(EMOTION_LAUGHING);
					//caster[1-i]->updateEmotion(EMOTION_SURPRISED);
					// gameState:
					//	bit 1 -- caster[0] spellcard triger in client
					//	bit 2 -- caster[1] spellcard triger in client
					toClientData.gameState |= (1 << (1 + i));
				}

			}
			// Update the entities
			bugball[i].update(frameTime);
			moon[i].update(frameTime);
		}
	}
	if (caster[0]->getX() >= GAME_WIDTH / 2-32)
		caster[0]->setX(GAME_WIDTH/2 - 32);
	caster[0]->update(frameTime);
	if(caster[1]->getX() <= GAME_WIDTH/2)
		caster[1]->setX(GAME_WIDTH/2);
	caster[1]->update(frameTime);

	// if 2 or more players connected AND (only 1 visible OR round over)
	if (playerCount >= 2 && (casterCount <= 1 || roundOver))
		startTimerRun = true;
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CastOperations::ai()
{
	bugBallTracking();
}

//=============================================================================
// bug ball tracks an enemy caster automatically
//=============================================================================
void CastOperations::bugBallTracking() {
	D3DXVECTOR2 unitVector; // vector
	int j; // enemy
	// for all players
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (bugball[i].getActive()) {
			bugball[i].setThinkTimer(bugball[i].getThinkTimer() - frameTime);
			if (bugball[i].getThinkTimer() <= 0) {
				bugball[i].setThinkTimer(3.0f);
				// compute the unit vector
				j = 1 - i; // get the index of an enemy 
				// enemyPositionVector - bugBallPositionVector
				unitVector.x = (caster[j]->getX() - bugball[i].getX());
				unitVector.y = (caster[j]->getY() - bugball[i].getY());
				// normalize the vector

				if (abs(unitVector.x) > abs(unitVector.y)) {
					unitVector.y = 0;
				}
				else {
					unitVector.x = 0;
				}
				graphics->Vector2Normalize(&unitVector);

				// apply the direction vector multipled by the speed
				bugball[i].setVelocity(unitVector*BUG_BALL_SPEED);
			}
		}
	}
}

//=============================================================================
// Handle collisions
//=============================================================================
void CastOperations::collisions()
{
	VECTOR2 collisionVector;
	UCHAR sounds = toClientData.sounds; // get current sound states

	for (int i = 0; i < MAX_PLAYERS; i++)   // for all players
	{


		for (int j = 0; j < MAX_PLAYERS; j++)   // for all bugballs
		{
			if (i != j)  // don't collide with our own bugball
			{
				
				// if collision between caster and bugball
				if (caster[i]->collidesWith(bugball[j], collisionVector))
				{
					// during safety time, cancel the collision
					if (caster[i]->getSafetyTimeAfterDamage() <= 0) {
						caster[i]->damage(BUGBALL);
						bugball[j].setVisible(false);
						bugball[j].setActive(false);
						// change the state of the sound bit to play the sound
						if (sounds & (SATORI_DAMAGE_BIT << (2 * i) ))    // if bit was 1
							toClientData.sounds &= (0xFF ^ (SATORI_DAMAGE_BIT << (2*i)) ); // set 0
						else                            // bit was 0
							toClientData.sounds |= (SATORI_DAMAGE_BIT << (2*i)) ;     // set 1
					}
				}
			}
		}

		for (int j = 0; j < MAX_PLAYERS; j++)   // for all moons
		{
			if (i != j)  // don't collide with our own moon
			{
				// if collision between caster and moon
				if (caster[i]->collidesWith(moon[j], collisionVector))
				{
					// during safety time, cancel the collision
					if (caster[i]->getSafetyTimeAfterDamage() <= 0) {
						caster[i]->damage(MOON);
						moon[j].setVisible(false);
						moon[j].setActive(false);
						// change the state of the sound bit to play the sound
						if (sounds & (SATORI_DAMAGE_BIT << (2 * i)) )    // if bit was 1
							toClientData.sounds &= (0xFF ^ (SATORI_DAMAGE_BIT << (2 * i))); // set 0
						else                            // bit was 0
							toClientData.sounds |= (SATORI_DAMAGE_BIT << (2 * i));     // set 1
					}
				}
			}
		}

	}
}

//=============================================================================
// Render game items
//=============================================================================
void CastOperations::render()
{
	graphics->spriteBegin();                // begin drawing sprites
	menu.draw();
	graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// process console commands
//=============================================================================
void CastOperations::consoleCommand()
{
	command = console->getCommand();    // get command from console
	if (command == "")                   // if no command
		return;

	if (command == "help")              // if "help" command
	{
		console->print(" ");
		console->print("Console Commands:");
		console->print("~ - show/hide console");
		console->print("fps - toggle display of frames per second");
		console->print("gravity off - turns off planet gravity");
		console->print("gravity on - turns on planet gravity");
		console->print("port # - sets port number, CAUTION! Restarts server");
		return;
	}
	else if (command == "fps")
	{
		fpsOn = !fpsOn;                 // toggle display of fps
		if (fpsOn)
			console->print("fps On");
		else
			console->print("fps Off");
	}
	else if (command.substr(0, 4) == "port")
	{
		int newPort = atoi(command.substr(5).c_str());
		if (newPort > netNS::MIN_PORT && newPort < 65536)
		{
			port = newPort;             // set new port
			countDownOn = false;
			playerCount = 0;
			netTime = 0;
			initializeServer(port);     // re-initialize game server
			roundOver = true;
		}
		else
			console->print("Invalid port number");
	}
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void CastOperations::releaseAll()
{
	menuTexture.onLostDevice();
	gameTextures.onLostDevice();
	satoriTexture.onLostDevice();
	koishiTexture.onLostDevice();
	bugballTextures.onLostDevice();
	moonTexture.onLostDevice();

	Game::releaseAll();
	return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void CastOperations::resetAll()
{
	menuTexture.onResetDevice();
	gameTextures.onResetDevice();
	satoriTexture.onResetDevice();
	koishiTexture.onResetDevice();
	bugballTextures.onResetDevice();
	moonTexture.onResetDevice();
	Game::resetAll();
	return;
}

//   Network Functions   
//-----------------------------------------------------------------------------

//=============================================================================
// Initialize Server
//=============================================================================
int CastOperations::initializeServer(int port)
{
	std::stringstream ss;

	if (port < netNS::MIN_PORT)
	{
		console->print("Invalid port number");
		return netNS::NET_ERROR;
	}
	// ----- Initialize network stuff -----
	error = net.createServer(port, netNS::UDP);
	if (error != netNS::NET_OK)              // if error
	{
		console->print(net.getError(error));
		return netNS::NET_ERROR;
	}

	for (int i = 0; i < MAX_PLAYERS; i++)       // for all players
	{
		caster[i]->setActive(false);
		caster[i]->setConnected(false);
	}

	console->print("----- Server -----");
	net.getLocalIP(localIP);
	ss << "Server IP: " << localIP;
	console->print(ss.str());
	ss.str("");                             // clear stringstream
	ss << "Port: " << port;
	console->print(ss.str());
	return netNS::NET_OK;
}

//=============================================================================
// Do network communications
//=============================================================================
void CastOperations::communicate(float frameTime)
{
	// communicate with client
	// this function is not delayed so client response is as fast as possible
	doClientCommunication();

	// calculate elapsed time for network communications
	netTime += frameTime;
	if (netTime < netNS::NET_TIME)      // if not time to communicate
		return;
	netTime -= netNS::NET_TIME;

	// check for inactive clients, called every NET_TIME seconds
	checkNetworkTimeout();
}

//=============================================================================
// Check for network timeout
//=============================================================================
void CastOperations::checkNetworkTimeout()
{
	std::stringstream ss;

	for (int i = 0; i < MAX_PLAYERS; i++)       // for all players
	{
		if (caster[i]->getConnected())
		{
			caster[i]->incTimeout();               // timeout++
			// if communication timeout
			if (caster[i]->getTimeout() > netNS::MAX_ERRORS)
			{
				caster[i]->setConnected(false);
				ss << "***** Player " << i << " disconnected. *****";
				console->print(ss.str());
			}
		}
	}
}

//=============================================================================
// Do client communication
// Called by server to send game state to each client
//=============================================================================
void CastOperations::doClientCommunication()
{
	int playN;                  // player number we are communicating with
	int size;
	prepareDataForClient();     // prepare data for transmission to clients

	for (int i = 0; i < MAX_PLAYERS; i++)   // for all players
	{
		size = sizeof(toServerData);
		if (net.readData((char*)&toServerData, size, remoteIP) == netNS::NET_OK)
		{
			if (size > 0)                // if data received
			{
				playN = toServerData.playerN;
				if (playN == 255)       // if request to join game
				{
					clientWantsToJoin();
				}
				else if (playN >= 0 && playN < MAX_PLAYERS)  // if valid playerN
				{
					if (caster[playN]->getConnected()) // if this player is connected
					{
						if (caster[playN]->getActive()) // if this player is active
							caster[playN]->setButtons(toServerData.buttons);
						size = sizeof(toClientData);
						// send player the latest game data
						net.sendData((char*)&toClientData, size, caster[playN]->getNetIP());
						caster[playN]->setTimeout(0);
						caster[playN]->setCommWarnings(0);
					}
				}
			}
		}
		else    // no more incomming data
		{
			break;
		}
	}
}

//=============================================================================
// Prepare data to send to client. It contains information on all players.
//=============================================================================
void CastOperations::prepareDataForClient()
{
	for (int i = 0; i < MAX_PLAYERS; i++)       // for all players
	{
		toClientData.player[i].casterData = caster[i]->getNetData();
		toClientData.player[i].bugballData = bugball[i].getNetData();
		toClientData.player[i].moonData = moon[i].getNetData();
	}
}

//=============================================================================
// Client is requesting to join game
//=============================================================================
void CastOperations::clientWantsToJoin()
{
	std::stringstream ss;
	int size;
	int status;

	connectResponse.number = 255;       // set to invalid player number

	if (playerCount == 0)                // if no players currently in game
	{
		roundOver = true;               // start a new round
	}

	console->print("Player requesting to join.");
	// find available player position to use
	for (int i = 0; i < MAX_PLAYERS; i++)        // search all player positions
	{
		if (caster[i]->getConnected() == false)    // if this position available
		{
			caster[i]->setConnected(true);
			caster[i]->setTimeout(0);
			caster[i]->setCommWarnings(0);
			caster[i]->setNetIP(remoteIP);     // save player's IP
			caster[i]->setCommErrors(0);       // clear old errors
			// send SERVER_ID and player number to client
			strcpy_s(connectResponse.response, netNS::SERVER_ID);
			connectResponse.number = (UCHAR)i;
			size = sizeof(connectResponse);
			status = net.sendData((char*)&connectResponse, size, remoteIP);
			if (status == netNS::NET_ERROR)
			{
				console->print(net.getError(status));   // display error message
				return;
			}
			toServerData.playerN = i;       // clear join request from input buffer                
			ss << "Connected player as number: " << i;
			console->print(ss.str());
			return;                         // found available player position
		}
	}
	// send SERVER_FULL to client
	strcpy_s(connectResponse.response, netNS::SERVER_FULL);
	size = sizeof(connectResponse);
	status = net.sendData((char*)&connectResponse, size, remoteIP);
	console->print("Server full.");
}


