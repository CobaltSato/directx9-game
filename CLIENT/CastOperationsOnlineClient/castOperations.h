//=============================================================================
// Cast Operations Online Client
// castOperations.h
// This is a core game class
//=============================================================================


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
#include "conversation.h"
#include "textDX.h"

namespace castOperationsNS
{
    const char FONT[] = "Arial Bold";  // font
    const int FONT_SIZE = 24;     // font height
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;
    const COLOR_ARGB CASTER1_COLOR = graphicsNS::PURPLE;
    const COLOR_ARGB CASTER2_COLOR = graphicsNS::GREEN;

    const int HEALTHBAR_Y = 30;
    const int CASTER1_HEALTHBAR_X = satoriConstantsNS::FACE_WIDTH + 5;
    const int HEALTHBAR_WIDTH = 64;
	const int CASTER2_HEALTHBAR_X = GAME_WIDTH - koishiConstantsNS::FACE_WIDTH -HEALTHBAR_WIDTH - 5;
    const int HEALTHBAR_START_FRAME = 22;
    const int HEALTHBAR_END_FRAME = 22;
    const int HEALTHBAR_HEIGHT = 32;

	const float COUNT_DOWN = 8.0f;          // count down from 5
    const int ROUND_TIME = 5;           // time until new round starts
    const int HEALTHBAR_TEXTURE_COLS = 4;   // health bar is a 4 column texture

    // Game types
    const int LOCAL = 0;            // two players on same computer
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

    const int CONNECT_TIMEOUT = 10; // seconds to wait when connecting
    const int JOIN_TIME = 5;        // seconds between join attempts
    // Game State bits
    const int ROUND_START_BIT = 0x01;
    // Network, sounds for client to play
    const int ENGINE1_BIT       = 0x01; // Bit 0 = engine1      1=on, 0=off
    const int ENGINE2_BIT       = 0x02; // Bit 1 = engine2      1=on, 0=off
    const int CHEER_BIT         = 0x04; // Bit 2 = cheer        state change
    const int COLLIDE_BIT       = 0x08; // Bit 3 = collide      state change
    const int EXPLODE_BIT       = 0x10; // Bit 4 = explode      state change
    const int BUGBALL_CRASH_BIT = 0x20; // Bit 5 = bugballCrash state change
    const int BUGBALL_FIRE_BIT  = 0x40; // Bit 6 = bugballFire  state change
    const int BUGBALL_HIT_BIT   = 0x80; // Bit 7 = bugballHit   state change
	const int SHIELD_BIT       = 0x08; // Bit 3 = collide       state change
    const int SATORI_FIRE_BIT     = 0x10; // Bit 4 = SatoriFire    state change
    const int SATORI_DAMAGE_BIT = 0x20; // Bit 5 = SatoriDamage  state change
    const int KOISHI_FIRE_BIT  = 0x40; // Bit 6 = KoishiFire    state change
    const int KOISHI_DAMAGE_BIT   = 0x80; // Bit 7 = KoishiDamage  state change

	const int SATORI_DESCRIPTION_X = 30;
	const int SATORI_DESCRIPTION_Y = 80;
	const std::string SATORI_SPELL_DESCRIPTION= "バリアーを発動中…";
	const int KOISHI_DESCRIPTION_X = GAME_WIDTH - 200;
	const int KOISHI_DESCRIPTION_Y = SATORI_DESCRIPTION_Y;
	const std::string KOISHI_SPELL_DESCRIPTION= "ブラインドを発動中…";
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
    CasterStc     casterData;
    BugballStc  bugballData;
	MoonStc moonData;
};

// ToClientStc is the structure that is sent from the server to each client. 
struct ToClientStc 
{
    Player  player[castOperationsNS::MAX_PLAYERS];
    // game state
    // Bit 0 = roundStart
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
// CastOperations is the class we create, it inherits from the Game class
//=============================================================================
class CastOperations : public Game
{
private:
	DISALLOW_COPY_AND_ASSIGN(CastOperations);
    // game textures
	//------------------------------------------------------------------
	TextureManager menuTexture;
	TextureManager nebulaTexture;
	TextureManager gameTextures;  
	TextureManager messageboxTextures;
	TextureManager bugballTexture,moonTexture;
	// casters textures
	TextureManager satoriTexture, koishiTexture;
	TextureManager satoriStandingTextures, koishiStandingTextures;
	TextureManager satoriFaceTexture, koishiFaceTexture;
	TextDX textDX;
	TextureManager tileTextures;
	char board[BOARD_HEIGHT][BOARD_WIDTH];

	// game objects
	//------------------------------------------------------------------
    Bugball bugball[castOperationsNS::MAX_PLAYERS];  
    Moon moon[castOperationsNS::MAX_PLAYERS];   // bugballs
    Image   nebula;             // backdrop image
    Image   menu;               // menu image
	Conversation conversation;
    Bar     healthBar;          // health bar for casters
	Image satoriStanding, koishiStanding;
	Image satoriFace, koishiFace;
	Satori satori;
	Koishi koishi;
	// the object array for polymorphism usage
    Caster*    caster[castOperationsNS::MAX_PLAYERS]; 
	Image   planet;
	Image	boardSquare;

	// game flags
    bool    menuOn; // Should the screen displyed?
    bool    countDownOn;        // true when count down is displayed
    bool    roundOver;          // true when round is over

	// timer
    float   countDownTimer; // for conversation session
    float   roundTimer;         // time until new round starts
	float	messageTimer;  // a conversation updates upon this timer

    int     playerN;            // our player number

    // Network variables
	//------------------------------------------------------------------
    Net  net;                   // network object
    char protocol;              // TCP or UDP
    char remoteIP[castOperationsNS::BUFSIZE];     // Remote IP address or hostname
    int  port;                  // port number
    char localIP[16];           // Local IP address as dotted quad; nnn.nnn.nnn.nnn
    char netbuf[castOperationsNS::BUFSIZE];       // network receive
    char keybuf[castOperationsNS::BUFSIZE];       // keyboard input
    int error; 
    int lastError; 
    int sizeXmit;               // transmit size
    int sizeRecv;               // receive size
    int size;
    ToClientStc toClientData;   // data struct sent to client from server
    ToServerStc toServerData;   // data struct sent to server from client
    ConnectResponse connectResponse;
    UINT commErrors;
    UINT commWarnings;
    bool tryToConnect;
    bool clientConnected;
    UCHAR buttonState;          // current state of player buttons
    UCHAR soundState;           // current sound state
    UCHAR gameState;            // current game state
    float netTime;              // network timer

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

    void consoleCommand(); // process console command
    void roundStart();  // start a new round of play

	// memory management functions
    void releaseAll();
    void resetAll();

    // Network functions
    void communicate(float frameTime);
    void checkNetworkTimeout();
    //void prepareDataForClient(); // server side function
    //void doClientCommunication(); // server side function
    void sendInfoToServer();
    void getInfoFromServer();
    //void clientWantsToJoin(); // server side function
    void connectToServer();
};

#endif
