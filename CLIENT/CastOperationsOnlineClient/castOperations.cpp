//=============================================================================
// Cast Operations Online Client
// castOperations.cpp
//=============================================================================

#include "castOperations.h"
using namespace castOperationsNS;

// TEST_MODE flag
// if true, local host and default dp port are used for setting up
bool TEST_MODE = true;

//=============================================================================
// Constructor
//=============================================================================
CastOperations::CastOperations()
{
	// just initialize fields
    menuOn = true;
    countDownOn = false;
    initialized = false;
    port = netNS::DEFAULT_PORT;
    netTime = 0;
    playerN = 0;                // assigned by server
    error     = netNS::NET_OK; 
    lastError = netNS::NET_OK; 
    sizeXmit=0;                 // transmit size
    sizeRecv=0;                 // receive size
    size=0;
    tryToConnect = false;
    clientConnected = false;
    commErrors = 0;
    commWarnings = 0;
    buttonState = 0;
    soundState = 0;
    gameState = 0;
	messageTimer = 2.5f;

	for (int row = 0; row < BOARD_HEIGHT; row++)
		for (int col = 0; col < BOARD_WIDTH; col++)
			board[row][col] = 0;
	
	// for polymorphism usage
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

	// initialize DirectX fonts
	textDX.initialize(graphics, castOperationsNS::FONT_SIZE, false, false, castOperationsNS::FONT);
	textDX.setFontColor(castOperationsNS::FONT_COLOR);


	initializeTextures(); // throws GameError

	initializeObjects(); // throws GameError
}

//=============================================================================
// initialize the all textures in the game
//=============================================================================
void CastOperations::initializeTextures() {
	// menu texture
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// board Texture
	if (!tileTextures.initialize(graphics, BOARD_IMAGE,TRANSCOLOR_WHITE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures"));

	// nebula texture
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// main game textures
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// messagebox texture
	if (!messageboxTextures.initialize(graphics, MESSAGEBOX_IMAGE, TRANSCOLOR_WHITE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// satori,koishi texture
	if (!satoriTexture.initialize(graphics, SATORI_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));
	if (!koishiTexture.initialize(graphics, KOISHI_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// standing image textures
	if (!satoriStandingTextures.initialize(graphics, SATORI_STANDING_IMAGE, TRANSCOLOR_BLACK))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing satori textures"));
	if (!koishiStandingTextures.initialize(graphics, KOISHI_STANDING_IMAGE, TRANSCOLOR_BLACK))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing koishi textures"));

	// standing image textures
	if (!satoriFaceTexture.initialize(graphics, SATORI_FACE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing satori textures"));
	if (!koishiFaceTexture.initialize(graphics, KOISHI_FACE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing satori textures"));

	// bugball texture
	if (!bugballTexture.initialize(graphics, BUGBALL_IMAGE, SETCOLOR_ARGB(255, 0, 0, 0)))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// moon texture
	if (!moonTexture.initialize(graphics, MOON_IMAGE, SETCOLOR_ARGB(255, 255, 255, 255)))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));
}

//=============================================================================
// initialize the all objects in the game
//=============================================================================
void CastOperations::initializeObjects(){
    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // nebula image
    if (!nebula.initialize(graphics,0,0,0,&nebulaTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	nebula.setScale(2.0f);

	// Board square image
	boardSquare.initialize(graphics,
		BOARD_TEXTURE_SIZE,
		BOARD_TEXTURE_SIZE,
		BOARD_TEXTURE_COLS,
		&tileTextures);
	boardSquare.setFrames(BOARD_FRAME, BOARD_FRAME);
	boardSquare.setCurrentFrame(BOARD_FRAME);


    // casters
    if (!caster[0]->initialize(this, &satoriTexture,SATORI_DOT_STC,&satoriFaceTexture,SATORI_FACE_STC,&gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing caster1"));
    caster[0]->setFrames(casterNS::CASTER_NORMAL_FIRST,casterNS::CASTER_NORMAL_LAST);
    caster[0]->setCurrentFrame(casterNS::CASTER_FIRST_FRAME);

    if (!caster[1]->initialize(this,  &koishiTexture,KOISHI_DOT_STC,&koishiFaceTexture,KOISHI_FACE_STC,&gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing caster2"));
    caster[1]->setFrames(casterNS::CASTER_NORMAL_FIRST,casterNS::CASTER_NORMAL_LAST);
    caster[1]->setCurrentFrame(casterNS::CASTER_FIRST_FRAME);

	// messagebox
    if (!conversation.initializeConversation(graphics, &messageboxTextures,&satoriStandingTextures,&koishiStandingTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing caster2"));

    // bugballs
    if (!bugball[0].initialize(this, bugballNS::WIDTH, bugballNS::HEIGHT, bugballNS::TEXTURE_COLS, &bugballTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bugball1"));
    bugball[0].setFrames(bugballNS::FIRST_FRAME, bugballNS::LAST_FRAME);
    bugball[0].setCurrentFrame(bugballNS::FIRST_FRAME);
    bugball[0].setColorFilter(SETCOLOR_ARGB(255,128,128,255));   // light blue
    if (!bugball[1].initialize(this, bugballNS::WIDTH, bugballNS::HEIGHT, bugballNS::TEXTURE_COLS, &bugballTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bugball2"));
    bugball[1].setFrames(bugballNS::FIRST_FRAME, bugballNS::LAST_FRAME);
    bugball[1].setCurrentFrame(bugballNS::FIRST_FRAME);
    bugball[1].setColorFilter(SETCOLOR_ARGB(255,255,255,64));     // light yellow

    // moons
    if (!moon[0].initialize(this, moonNS::WIDTH, moonNS::HEIGHT, moonNS::TEXTURE_COLS, &moonTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moon1"));
    moon[0].setFrames(moonNS::FIRST_FRAME, moonNS::LAST_FRAME);
    moon[0].setCurrentFrame(moonNS::FIRST_FRAME);
    if (!moon[1].initialize(this, moonNS::WIDTH, moonNS::HEIGHT, moonNS::TEXTURE_COLS, &moonTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moon2"));
    moon[1].setFrames(moonNS::FIRST_FRAME, moonNS::LAST_FRAME);
    moon[1].setCurrentFrame(moonNS::FIRST_FRAME);

    // health bar
    healthBar.initialize(graphics, &gameTextures,
		0, castOperationsNS::HEALTHBAR_Y, 
		2.0f, graphicsNS::WHITE);
	healthBar.setScale(2.0f);

       return;
}

//=============================================================================
// Start a new round of play
//=============================================================================
void CastOperations::roundStart()
{
    // Start casters on opposite sides of planet in stable clockwise orbit
    caster[0]->setX(GAME_WIDTH/4 - casterNS::WIDTH);
    caster[1]->setX(GAME_WIDTH - GAME_WIDTH/4);
    caster[0]->setY(GAME_HEIGHT/2 - casterNS::HEIGHT);
    caster[1]->setY(GAME_HEIGHT/2);
    caster[0]->setVelocity(VECTOR2(0,0));
    caster[1]->setVelocity(VECTOR2(0,0));

    countDownTimer = castOperationsNS::COUNT_DOWN;
    countDownOn = true;
	audio->playCue("bgm");
}

//=============================================================================
// Update all game items
//=============================================================================
void CastOperations::update()
{
	boardSquare.update(frameTime);
    buttonState = 0;        // clear network button state
	if (satori.getHealth() <= 0)
		conversation.koishiWon();
	if (koishi.getHealth() <= 0)
		conversation.satoriWon();
    if (menuOn)
    {
        if (input->anyKeyPressed())
        {
            menuOn = false;
            input->clearAll();
            tryToConnect = true;    // connect to game server
        }
    } 
    else if(countDownOn) // || conversation.getHasRead() == false)
    {
        console->hide();
        countDownTimer -= frameTime;
		messageTimer -= frameTime;
		if (messageTimer <= 0)
		{
			messageTimer = 2.5f;
			conversation.read();
		}
        if(countDownTimer <= 0)
        {
            countDownOn = false;
        }
    } 
    else 
    {
		nebula.setX(nebula.getX() - frameTime * nebula.getWidth() / 50);
		nebula.setY(nebula.getY() + frameTime * nebula.getHeight() / 50);
		// Wrap nebula image around at edge
		// if left edge of nebula > screen left edge
		if (nebula.getX() > 0)
			// move nebula image left by SPACE_WIDTH
			nebula.setX(nebula.getX() - nebula.getWidth());
		// if nebula image off screen left
		if (nebula.getX() < -nebula.getWidth())
			// move nebula image right by SPACE_WIDTH
			nebula.setX(nebula.getX() + nebula.getWidth());
		// if top edge of nebula > screen top edge
		if (nebula.getY() > 0)
			// move nebula image up by SPACE_HEIGHT
			nebula.setY(nebula.getY() - nebula.getHeight());
		// if nebula image off screen top
		if (nebula.getY() < -nebula.getHeight())
			// move nebula image down by SPACE_IMAGE
			nebula.setY(nebula.getY() + nebula.getHeight());

		for (int i = 0; i < MAX_PLAYERS; i++)       // for all players
		{
			if (playerN == i) {       // if we are player i
				// if engine on
				//if (input->isKeyDown(CASTER_FORWARD_KEY) || input->getGamepadDPadUp(1))
				//	buttonState |= FORWARD_BIT;
				if (input->isKeyDown(CASTER_RIGHT_KEY) || input->getGamepadDPadUp(1))
					buttonState |= RIGHT_BIT;
				if (input->isKeyDown(CASTER_LEFT_KEY) || input->getGamepadDPadUp(1))
					buttonState |= LEFT_BIT;
				if (input->isKeyDown(CASTER_UP_KEY) || input->getGamepadDPadUp(1))
					buttonState |= UP_BIT;
				if (input->isKeyDown(CASTER_DOWN_KEY) || input->getGamepadDPadUp(1))
					buttonState |= DOWN_BIT;
			}

			// if she fires
			if (input->isKeyDown(CASTER_FIRE_KEY) || input->getGamepadA(1))
				buttonState |= FIRE_BIT;

			// if she casts a speel
			if (input->isKeyDown(CASTER_SPELL_KEY) || input->getGamepadA(1))
				buttonState |= SPELL_BIT;

			// if she charges
			if (input->isKeyDown(CASTER_CHARGE_KEY) || input->getGamepadA(1))
				buttonState |= CHARGE_BIT;

			// Update the entities
			caster[i]->update(frameTime);
			bugball[i].update(frameTime);
			moon[i].update(frameTime);
		}
	}

}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CastOperations::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void CastOperations::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void CastOperations::render()
{
	graphics->spriteBegin();                // begin drawing sprites

	float x = nebula.getX();
	float y = nebula.getY();

	// Wrap nebula image around at edges
	nebula.draw();                           // draw at current location
	// if nebula image right edge visible
	if (nebula.getX() < -nebula.getWidth() + (int)GAME_WIDTH)
	{
		nebula.setX(nebula.getX() + nebula.getWidth()); // wrap around to left edge
		nebula.draw();                           // draw again
	}
	// if nebula image bottom edge visible
	if (nebula.getY() < -nebula.getHeight() + (int)GAME_HEIGHT)
	{
		nebula.setY(nebula.getY() + nebula.getHeight()); // wrap around to top edge
		nebula.draw();                           // draw again
		nebula.setX(x);                          // restore x position
		// if nebula image right edge visible
		// wrap around to left edge
		if (x < -nebula.getWidth() + (int)GAME_WIDTH)
			nebula.draw();                       // draw again
	}
	nebula.setX(x);      // restore x position
	nebula.setY(y);      // restore y position


	// display health bars
	healthBar.setY(16);
	healthBar.setDegrees(0);
	healthBar.set(caster[0]->getCharged() / 10 * 100);
	healthBar.setX((float)castOperationsNS::CASTER1_HEALTHBAR_X);
	healthBar.draw(castOperationsNS::CASTER1_COLOR);
	healthBar.set(100);
	healthBar.draw(castOperationsNS::CASTER1_COLOR & graphicsNS::ALPHA25);

	healthBar.set(caster[1]->getCharged() / 10 * 100);
	healthBar.setX((float)castOperationsNS::CASTER2_HEALTHBAR_X);
	healthBar.setY(16);
	healthBar.draw(castOperationsNS::CASTER2_COLOR);
	healthBar.set(100);
	healthBar.setX((float)castOperationsNS::CASTER2_HEALTHBAR_X);
	healthBar.setY(16);
	healthBar.draw(castOperationsNS::CASTER2_COLOR &  graphicsNS::ALPHA25);

	float checkerY = 0;

	graphics->spriteBegin();
	// Draw checkerboard
	for (int row = 0; row < BOARD_HEIGHT; row++)
	{
		for (int col = 0; col < BOARD_WIDTH; col++)
		{
			// Orthogonal Grid draw
			boardSquare.setX((float)(BOARD_X + (col* BOARD_TEXTURE_SIZE)));
			boardSquare.setY((float)(BOARD_Y + (row* BOARD_TEXTURE_SIZE / 2)));

			if (col <= 2) {       // rows and cols are alternating colors
				boardSquare.setCurrentFrame(0);
				boardSquare.draw();
			}
			else {
				boardSquare.setCurrentFrame(1);
				boardSquare.draw();
			}
		}
	}

	if (satori.getShieldTime() > 0) {
		satori.updateEmotion(EMOTION_LAUGHING);
		koishi.updateEmotion(EMOTION_SURPRISED);
		textDX.print(
			castOperationsNS::SATORI_SPELL_DESCRIPTION,
			castOperationsNS::SATORI_DESCRIPTION_X,
			castOperationsNS::SATORI_DESCRIPTION_Y
		);
	}

	if (koishi.getInvisibleTimer() > 0) {
		koishi.updateEmotion(EMOTION_LAUGHING);
		satori.updateEmotion(EMOTION_SURPRISED);
		textDX.print(
			castOperationsNS::KOISHI_SPELL_DESCRIPTION,
			castOperationsNS::KOISHI_DESCRIPTION_X,
			castOperationsNS::KOISHI_DESCRIPTION_Y
		);
		if (playerN == 1) {
			koishi.draw();
		}
		else {
			koishi.vanishingDraw();
		}
	}
	else {
		koishi.draw();
	}

	satori.draw();                           // draw the casters

	bugball[0].draw(graphicsNS::FILTER);      // draw the bugballs using colorFilter
	bugball[1].draw(graphicsNS::FILTER);
	moon[0].draw(graphicsNS::FILTER);      // draw the moons using colorFilter
	moon[1].draw(graphicsNS::FILTER);


	if (menuOn)
		menu.draw();

	if (countDownOn)
	{
		conversation.write();
	}
	else {
		textDX.print(
			std::to_string(static_cast<int>(satori.getHealth())),
			satoriConstantsNS::FACE_WIDTH + 5,
			5
		);
		textDX.print(
			std::to_string(static_cast<int>(koishi.getHealth())),
			GAME_WIDTH - koishiConstantsNS::FACE_WIDTH - 24 * 2,
			5
		);
	}


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
		console->print("connect - connect to game server");
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
	else if (command == "connect")
		tryToConnect = true;        // connect to game server
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void CastOperations::releaseAll()
{
	// release vidio memory on the reversed order
	menuTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	gameTextures.onLostDevice();
	messageboxTextures.onLostDevice();
	satoriTexture.onLostDevice();
	koishiTexture.onLostDevice();
	satoriStandingTextures.onLostDevice();
	koishiStandingTextures.onLostDevice();
	satoriFaceTexture.onLostDevice();
	koishiFaceTexture.onLostDevice();
	bugballTexture.onLostDevice();
	moonTexture.onLostDevice();
	textDX.onLostDevice();
	conversation.getFontSmall()->onLostDevice();
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
	nebulaTexture.onResetDevice();
	gameTextures.onResetDevice();
	nebulaTexture.onResetDevice();
	messageboxTextures.onResetDevice();
	satoriTexture.onResetDevice();
	koishiTexture.onResetDevice();
	satoriStandingTextures.onResetDevice();
	koishiStandingTextures.onResetDevice();
	satoriFaceTexture.onResetDevice();
	koishiFaceTexture.onResetDevice();
	textDX.onResetDevice();
	conversation.getFontSmall()->onResetDevice();
	Game::resetAll();
	return;
}

////////////////////////////
//   Network Functions    //
////////////////////////////

//=============================================================================
// Do network communications
//=============================================================================
void CastOperations::communicate(float frameTime)
{
	if (clientConnected)
		getInfoFromServer();    // get game state from server

	// calculate elapsed time for network communications
	netTime += frameTime;
	if (netTime < netNS::NET_TIME)      // if not time to communicate
		return;

	if (tryToConnect)            // if we are trying to connect to server
	{
		if (netTime < JOIN_TIME) // if not time to try
			return;
		connectToServer();      // attempt to connect to game server
		netTime -= JOIN_TIME;
		return;
	}
	else if (clientConnected)    // if connected
	{
		checkNetworkTimeout();  // check for disconnect from server
		sendInfoToServer();     // send player input to server
	}
	netTime -= netNS::NET_TIME;
}

//=============================================================================
// Attempt to connect to the game server.
// When a connection is established the game begins.
//=============================================================================
void CastOperations::connectToServer()
{
	static int step = 0;
	static float waitTime;  // seconds we have waited for a response from server
	int size;
	int newPort;
	std::string str;
	std::stringstream ss;

	if (clientConnected)     // if connected
	{
		tryToConnect = false;
		console->print("Currently connected");
		return;
	}

	switch (step)
	{
	case 0:
		console->print("----- CastOperations Client -----");
		console->print("Enter IP address or name of server:");
		console->clearInput();              // clear input text
		console->show();
		step = 1;
		break;

	case 1:
		if (TEST_MODE == false) {
			str = console->getInput();
			if (str == "")                       // if no address entered
				return;
			strcpy_s(remoteIP, str.c_str());    // copy address to remoteIP
			console->clearInput();              // clear input text
		}
		else {
			str = "127.0.0.1";
			strcpy_s(remoteIP, str.c_str());    // copy address to remoteIP
		}
		console->print("Enter port number, 0 selects default: ");
		step = 2;
		break;

	case 2:
		if (TEST_MODE == false) {
			str = console->getInput();
			if (str == "")
				return;
			newPort = atoi(str.c_str());
		}
		else {
			newPort = 0;
		}
		if (newPort == 0)
			newPort = netNS::DEFAULT_PORT;
		if (newPort > netNS::MIN_PORT && newPort < 65536)
		{
			port = newPort;
			step = 3;
		}
		else
			console->print("Invalid port number");
		console->clearInput();
		break;

	case 3:
		// create UDP client
		error = net.createClient(remoteIP, port, netNS::UDP);
		if (error != netNS::NET_OK)          // if error
		{
			console->print(net.getError(error));    // display error message
			tryToConnect = false;
			step = 0;
			return;
		}
		// send request to join the server
		console->print("Attempting to connect with server."); // display message
		toServerData.playerN = 255;        // playerN=255 is request to join
		size = sizeof(toServerData);
		console->print("'Request to join' sent to server.");
		error = net.sendData((char*)&toServerData, size, remoteIP);
		console->print(net.getError(error));
		waitTime = 0;
		step = 4;
		break;

	case 4:
		waitTime += netTime;      // add time since last call to connect
		// if we timed out with no reponse to our request to join
		if (waitTime > CONNECT_TIMEOUT)
		{
			step = 3;               // send request again
			console->print("'Request to join' timed out.");
			//tryToConnect = false;
			return;
		}
		// read ConnectResponse from server
		size = sizeof(connectResponse);
		error = net.readData((char*)&connectResponse, size, remoteIP);
		if (error == netNS::NET_OK)     // if read was OK
		{
			if (size == 0)   // if no data received
				return;
			// if the server sent back the proper ID then we are connected
			if (strcmp(connectResponse.response, netNS::SERVER_ID) == 0)
			{
				if (connectResponse.number < MAX_PLAYERS)   // if valid player number
				{
					playerN = connectResponse.number;       // set my player number
					ss << "Connected as player number: " << playerN;
					console->print(ss.str());
					clientConnected = true;
					commErrors = 0;
					commWarnings = 0;
				}
				else
					console->print("Invalid player number received from server.");
			}
			else if (strcmp(connectResponse.response, netNS::SERVER_FULL) == 0)
				console->print("Server Full");
			else
			{
				console->print("Invalid ID from server. Server sent:");
				console->print(connectResponse.response);
			}
		}
		else        // read error
		{
			console->print(net.getError(error));
		}
		tryToConnect = false;
		step = 0;
	}
}

//=============================================================================
// Check for network timeout
//=============================================================================
void CastOperations::checkNetworkTimeout()
{
	if (!clientConnected)
		return;
	commErrors++;                         // increment timeout count
	if (commErrors > netNS::MAX_ERRORS)   // if communication timeout
	{
		clientConnected = false;
		console->print("***** Disconnected from server. *****");
		console->show();
	}
}

//=============================================================================
// Send the keypress codes to the server
//=============================================================================
void CastOperations::sendInfoToServer()
{
	int size;
	// prepare structure to be sent
	toServerData.buttons = buttonState;
	toServerData.playerN = playerN;
	// send data from client to server
	size = sizeof(toServerData);
	error = net.sendData((char*)&toServerData, size, remoteIP);
}

//=============================================================================
// Get toClientData from server.
// called by client to get game state from server
//=============================================================================
void CastOperations::getInfoFromServer()
{
	int size;
	size = sizeof(toClientData);
	int readStatus = net.readData((char *)&toClientData, size, remoteIP);
	if (readStatus == netNS::NET_OK && size > 0)
	{
		for (int i = 0; i < MAX_PLAYERS; i++)        // for all player positions
		{
			// load new data into each caster and bugball
			caster[i]->setNetData(toClientData.player[i].casterData);
			bugball[i].setNetData(toClientData.player[i].bugballData);
			moon[i].setNetData(toClientData.player[i].moonData);
		}

		// Game state
		// Bit 0 = roundStart
		// Bits 1-7 reserved for future use
		if ((toClientData.gameState & ROUND_START_BIT) &&
			countDownOn == false)
			roundStart();
		if (toClientData.gameState & (1 << 1))
			caster[0]->spellCard();
		if (toClientData.gameState & (1 << 2))
			caster[1]->spellCard();

		gameState = toClientData.gameState; // save new game state

		// Play sounds as intdicated by server
		// Momentary sounds are indicated by a state change. With a 
		// state change from 0 to 1 or 1 to 0 the sound is played
		// one time. Other sounds are indicated with 1=on, 0=off.
		// Bit 0 = cheer        state change
		// Bit 1 = collide      state change
		// Bit 2 = explode      state change
		// Bit 3 = engine1      1=on, 0=off
		// Bit 4 = engine2      1=on, 0=off
		// Bit 5 = bugballCrash state change
		// Bit 6 = Fire  state change
		// Bit 7 = Hit   state change

	// Bit 3 = Shield
	// Bit 4 = SatoriFire
	// Bit 5 = SatoriDamage
    // Bit 6 = SatoriFire  state change
    // Bit 7 = SatoriFIre   state chang
		if ((toClientData.sounds & CHEER_BIT) != (soundState & CHEER_BIT))
			audio->playCue(CHEER);
		if ((toClientData.sounds & COLLIDE_BIT) != (soundState & COLLIDE_BIT))
			audio->playCue(COLLIDE);
		//if ((toClientData.sounds & EXPLODE_BIT) != (soundState & EXPLODE_BIT))
		//	audio->playCue("koishiDamage");

		/*
		if (toClientData.sounds & ENGINE1_BIT)
			audio->playCue(ENGINE1);
		else
			audio->stopCue(ENGINE1);

		if (toClientData.sounds & ENGINE2_BIT)
			audio->playCue(ENGINE2);
		else
			audio->stopCue(ENGINE2);
		
		if ((toClientData.sounds & BUGBALL_CRASH_BIT) !=
			(soundState & BUGBALL_CRASH_BIT))
			audio->playCue(BUGBALL_CRASH);
			*/
		if ((toClientData.sounds & SATORI_FIRE_BIT) !=
			(soundState & SATORI_FIRE_BIT))
			audio->playCue("satoriFire");

		if ((toClientData.sounds & SATORI_DAMAGE_BIT) !=
			(soundState & SATORI_DAMAGE_BIT))
			audio->playCue("satoriDamage");

		if ((toClientData.sounds & KOISHI_FIRE_BIT) !=
			(soundState & KOISHI_FIRE_BIT))
			audio->playCue("koishiFire");

		if ((toClientData.sounds & KOISHI_DAMAGE_BIT) !=
			(soundState & KOISHI_DAMAGE_BIT))
			audio->playCue("koishiDamage");


		soundState = toClientData.sounds;      // save new sound states

		commErrors = 0;
		commWarnings = 0;
	}

}
