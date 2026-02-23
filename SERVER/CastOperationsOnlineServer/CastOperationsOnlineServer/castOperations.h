// Cast Operations Online Server
// castOperations.cpp

#ifndef _CASTOPERATIONS_H             // Prevent multiple definitions if this 
#define _CASTOPERATIONS_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "dashboard.h"
#include "caster.h"
#include "bugball.h"
#include "net.h"
#include "satori.h"
#include "koishi.h"
#include "moon.h"

namespace castOperationsNS
{
    const char FONT[] = "Arial Bold";  // font
    const int FONT_BIG_SIZE = 256;     // font height
    const int FONT_SCORE_SIZE = 48;
    const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const COLOR_ARGB CASTER1_COLOR = graphicsNS::BLUE;
    const COLOR_ARGB CASTER2_COLOR = graphicsNS::YELLOW;
    const int SCORE_Y = 10;
    const int SCORE1_X = 60;
    const int SCORE2_X = GAME_WIDTH-80;
    const int HEALTHBAR_Y = 30;
    const int CASTER1_HEALTHBAR_X = 40;
    const int CASTER2_HEALTHBAR_X = GAME_WIDTH-100;
    const int COUNT_DOWN_X = GAME_WIDTH/2 - FONT_BIG_SIZE/4;
    const int COUNT_DOWN_Y = GAME_HEIGHT/2 - FONT_BIG_SIZE/2;
    const int COUNT_DOWN = 5;           // count down from 5
    const int BUF_SIZE = 20;
    const int ROUND_TIME = 5;           // time until new round starts
    const int START_TIME = 5;           // delay until round timer starts
    const int MENU_TIME = 5;            // seconds menu is displayed
    // Game types
    const int CLIENT = 1;           // client in network game
    const int SERVER = 2;           // server in network game
    const int MAX_PLAYERS = 2;      // maximum number of network players
    // Network
    const int BUFSIZE = 256;
	const int LEFT_BIT = 0x01;      // player buttons
	const int UP_BIT = 0x02;
	const int RIGHT_BIT = 0x04;
	const int FIRE_BIT = 0x08;
	const int DOWN_BIT = 0x10;
	const int CHARGE_BIT = 0x20;
	const int SPELL_BIT = 0x40;

    // Game State bits
    const int ROUND_START_BIT = 0x01;
    // Network, sounds for client to play
    const int BUGBALL_CRASH_BIT = 0x20; // Bit 5 = SatoriDamage  state change
    const int BUGBALL_FIRE_BIT  = 0x40; // Bit 6 = KoishiFire    state change
    const int BUGBALL_HIT_BIT   = 0x80; // Bit 7 = KoishiDamage   state change

	const int SHIELD_BIT       = 0x08; // Bit 3 = collide       state change
    const int SATORI_FIRE_BIT     = 0x10; // Bit 4 = SatoriFire    state change
    const int SATORI_DAMAGE_BIT = 0x20; // Bit 5 = SatoriDamage  state change
    const int KOISHI_FIRE_BIT  = 0x40; // Bit 6 = KoishiFire    state change
    const int KOISHI_DAMAGE_BIT   = 0x80; // Bit 7 = KoishiDamage  state change

}

//=============================================================================
// network play structures
//=============================================================================

// Sent to client in response to connection request
struct ConnectResponse
{
    char response[netNS::RESPONSE_SIZE];    // server response
    UCHAR   number;                         // player number if connected
};

// Player describes the caster and bugball for one player.
struct Player
{
    CasterStc	casterData;
    BugballStc  bugballData;
	MoonStc moonData;
};

// ToClientStc is the structure that is sent from the server to each client. 
struct ToClientStc 
{
    Player  player[castOperationsNS::MAX_PLAYERS];
    // game state
    // Bit 0 = roundStart
    // Bit 1 = satoriSpells
    // Bit 2 = koishiSpells
    // Bits 1-7 reserved for future use
    UCHAR   gameState;
    // sound to play   
	// Bit 3 = Shield
	// Bit 4 = SatoriFire
	// Bit 5 = SatoriDamage
    // Bit 6 = KoishiFire 
    // Bit 7 = KoishiDamage 
	UCHAR   sounds;
};

// ToServerStc is the structure that is sent from the client to the server.
struct ToServerStc 
{
    // current key presses
    UCHAR buttons;      // bit 0=Left, 1=Forward, 2=Right, 3=Fire
    UCHAR playerN;      // player number
};

//=============================================================================
// class CastOperations
// This class is the core of the game
//
//=============================================================================
class CastOperations : public Game
{
private:
	DISALLOW_COPY_AND_ASSIGN(CastOperations);
    // game textures
	TextureManager menuTexture, nebulaTexture, gameTextures; 
	TextureManager satoriTexture, koishiTexture;
	TextureManager bugballTextures,moonTexture;

	// game objects
	Satori satori; 
	Koishi koishi;
    Caster*    caster[castOperationsNS::MAX_PLAYERS]; // for polymorphism usage
    Bugball bugball[castOperationsNS::MAX_PLAYERS];   // bugballs
    Moon moon[castOperationsNS::MAX_PLAYERS];   // bugballs
    Image   menu;               // menu image

	// game flags
    bool    menuOn;
    bool    countDownOn;        // true when count down is displayed
    bool    startTimerRun;      // true when start timer is running
    bool    roundOver;          // true when round is over

	// timers
    float   menuTimer;          // menu display timer
    float   countDownTimer;
    float   startTimer;
    float   roundTimer;         // time until new round starts

    // Network variables
	//==============================================================================
    Net  net;                   // network object
    char remoteIP[16];          // Remote IP address as dotted quad; nnn.nnn.nnn.nnn
    int  port;                  // port number
    char localIP[16];           // Local IP address as dotted quad; nnn.nnn.nnn.nnn
    UINT gameType;
    ToServerStc toServerData;
    ToClientStc toClientData;
    ConnectResponse connectResponse;
    int playerCount;            // number of players in game
    float netTime;
    int error;

public:
    CastOperations();
    virtual ~CastOperations();
    // Initialize the game
    void initialize(HWND hwnd);
	void initializeTextures();
	void initializeObjects();

	// core Game virtual functions
    void update();      
    void ai();           
    void collisions();   
    void render();       

	void bugBallTracking(); // bug ball tracks an enemy caster
    void roundStart();  // start a new round of play

	// memory management functions
    void releaseAll();
    void resetAll();

	// process console command
    void consoleCommand(); 

    // Network functions
	//==============================================================================
    void communicate(float frameTime);
    int  initializeServer(int port);
    void checkNetworkTimeout();
    void prepareDataForClient();
    void doClientCommunication();
    //void sendInfoToServer(); // client-side functions commented out
    //void getInfoFromServer();
    void clientWantsToJoin();
    //void connectToServer();
};
#endif
