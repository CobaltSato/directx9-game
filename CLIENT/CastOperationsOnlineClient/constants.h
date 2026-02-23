// Cast Operations Online Client
// constants.h
// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>

// Useful macros
//---------------------------------------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)
#define TRANSCOLOR_WHITE  SETCOLOR_ARGB(255,255,255,255)  // transparent color (magenta)
#define TRANSCOLOR_BLACK  SETCOLOR_ARGB(255,0,0,0)  // transparent color (magenta)
#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
	ClassName(const ClassName&); \
	void operator=(const ClassName&);

//                  Constants
//-------------------------------------------------------------------------------------------------------

// window settings
//-------------------------------------------------------------------------------------------------------
const char CLASS_NAME[] = "Cast Operations Online";
const char GAME_TITLE[] = "Cast Operations Online Client";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels
 
// game settings
//-------------------------------------------------------------------------------------------------------
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float FULL_HEALTH = 100;
const UINT  MOON_SIZE = 64;                     // size of moon images
const UINT  MOON1_FRAME = 12;                   // frame of moon1

// graphic image file settings
// ------------------------------------------------------------------------------------------------------
const char NEBULA_IMAGE[] =   "pictures\\orion.jpg";     // photo source NASA/courtesy of nasaimages.org 
const char TEXTURES_IMAGE[] = "pictures\\textures.png";  // game textures
const char MENU_IMAGE[] =     "pictures\\menu.png";      // menu texture
const char SATORI_IMAGE[] =     "pictures\\satori.png";      // satori texture
const char KOISHI_IMAGE[] =     "pictures\\koishi.png";      // komeiji texture
const char BUGBALL_IMAGE[] =     "pictures\\bugBall.png";      // bug ball texture
const char MESSAGEBOX_IMAGE[] =     "pictures\\messagebox.png";      // message box texture
const char MOON_IMAGE[] =     "pictures\\moon.png";      // moon texture
const char SATORI_STANDING_IMAGE[] =     "pictures\\satoriStanding.png";      // satori standing texture
const char KOISHI_STANDING_IMAGE[] =     "pictures\\koishiStanding.png";      // koishi standing texture
const char SATORI_FACE_IMAGE[] =     "pictures\\satoriFace.png";      // satori face texture
const char KOISHI_FACE_IMAGE[] =     "pictures\\koishiFace.png";      // koishi face texture
const char TEXTURES1_IMAGE[] = "pictures\\textures1.png";  // game textures
const char BOARD_IMAGE[] = "pictures\\tile.png";  // game textures

// text file settings
// --------------------------------
const std::string MESSAGE =     "text\\message.txt";      // default textfile

// audio file settings
// --------------------------------
// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Win\\waveBank.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\soundBank.xsb";

// audio cues
const char CHEER[]          = "cheer";
const char COLLIDE[]        = "collide";
const char EXPLODE[]        = "explode";
const char ENGINE1[]        = "engine1";
const char ENGINE2[]        = "engine2";
const char BUGBALL_CRASH[]  = "torpedoCrash";
const char BUGBALL_FIRE[]   = "torpedoFire";
const char BUGBALL_HIT[]    = "torpedoHit";
const char RAO[]    = "rao";
const char BGM[]    = "bgm";

// key mappings
//--------------------------------------------------------------------------------
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY  = VK_OEM_3;    // `~ key for U.S.
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key
const UCHAR CASTER_LEFT_KEY   = VK_LEFT;  // left arrow
const UCHAR CASTER_RIGHT_KEY  = VK_RIGHT; // right arrow
const UCHAR CASTER_UP_KEY = VK_UP;   // up arrow
const UCHAR CASTER_DOWN_KEY = VK_DOWN;   // up arrow
const UCHAR CASTER_FIRE_KEY    = 0x5A; // shot
const UCHAR CASTER_CHARGE_KEY    = 0x43; // shot
const UCHAR CASTER_SPELL_KEY    = 0x58; // shot

/* test */
const int BOARD_FRAME = 0;
const int BOARD_WIDTH = 6;
const int BOARD_HEIGHT = 3;
const int BOARD_TEXTURE_SIZE = 64;
const int BOARD_TEXTURE_COLS = 2;
const int BOARD_X = GAME_WIDTH / 2 - BOARD_TEXTURE_SIZE * BOARD_WIDTH / 2;
const int BOARD_X_MAX = BOARD_X + BOARD_WIDTH * BOARD_TEXTURE_SIZE / 2;
const int BOARD_Y = GAME_HEIGHT / 2;
const int BOARD_Y_MAX = GAME_HEIGHT / 2 + BOARD_HEIGHT * BOARD_TEXTURE_SIZE / 2;

// Object settings
//---------------------------------------------------------------------------------
// ImageStc : useful structure for initializing texture manager
struct ImageStc {
	int WIDTH;
	int HEIGHT;
	float X;
	float Y;
	int NCOLS;
	int FRAME_FIRST;
	int FRAME_LAST;
};

// satori data setting
namespace satoriConstantsNS{
	// dot picture
	const int DOT_WIDTH = 32;
	const int DOT_HEIGHT = 32;
	const int DOT_X = GAME_WIDTH/4  - DOT_WIDTH;
    const int DOT_Y = GAME_HEIGHT/2 - DOT_HEIGHT + 60;
	const int DOT_NCOLS = 3;
	const int DOT_START_FRAME = 12;
	const int DOT_END_FRAME = 14;

	// face picture
	const int FACE_WIDTH = 64;
	const int FACE_HEIGHT = 64;
	const int FACE_X = 0;
	const int FACE_Y = 0;
	const int FACE_NCOLS = 3;
	const int FACE_START_FRAME = 5;
	const int FACE_END_FRAME = 5;
};
const ImageStc SATORI_DOT_STC{
	satoriConstantsNS::DOT_WIDTH,
	satoriConstantsNS::DOT_HEIGHT,
	satoriConstantsNS::DOT_X,
	satoriConstantsNS::DOT_Y,
	satoriConstantsNS::DOT_NCOLS,
	satoriConstantsNS::DOT_START_FRAME,
	satoriConstantsNS::DOT_END_FRAME
};
const ImageStc SATORI_FACE_STC {
	satoriConstantsNS::FACE_WIDTH,
	satoriConstantsNS::FACE_HEIGHT,
	satoriConstantsNS::FACE_X,
	satoriConstantsNS::FACE_Y,
	satoriConstantsNS::FACE_NCOLS,
	satoriConstantsNS::FACE_START_FRAME,
	satoriConstantsNS::FACE_END_FRAME
};

// koishi data setting
namespace koishiConstantsNS {
	// dot picture
	const int DOT_WIDTH = 32;
	const int DOT_HEIGHT = 32;
    const int DOT_X = GAME_WIDTH - GAME_WIDTH/4;
    const int DOT_Y = GAME_HEIGHT/2 + 60;
	const int DOT_NCOLS = 3;
	const int DOT_DEFAULT_FRAME = 12;
	const int DOT_START_FRAME = 12;
	const int DOT_END_FRAME = 14;

	// face picture
	const int FACE_WIDTH = 64;
	const int FACE_HEIGHT = 64;
	const int FACE_X = GAME_WIDTH - FACE_WIDTH;
	const int FACE_Y = 0;
	const int FACE_NCOLS = 3;
	const int FACE_DEFAULT_FRAME = 5;
	const int FACE_START_FRAME = 5;
	const int FACE_END_FRAME = 5;
};
const ImageStc KOISHI_DOT_STC{
	koishiConstantsNS::DOT_WIDTH,
	koishiConstantsNS::DOT_HEIGHT,
	koishiConstantsNS::DOT_X,
	koishiConstantsNS::DOT_Y,
	koishiConstantsNS::DOT_NCOLS,
	koishiConstantsNS::DOT_START_FRAME,
	koishiConstantsNS::DOT_END_FRAME
};
const ImageStc KOISHI_FACE_STC{
	koishiConstantsNS::FACE_WIDTH,
	koishiConstantsNS::FACE_HEIGHT,
	koishiConstantsNS::FACE_X,
	koishiConstantsNS::FACE_Y,
	koishiConstantsNS::FACE_NCOLS,
	koishiConstantsNS::FACE_START_FRAME,
	koishiConstantsNS::FACE_END_FRAME
};

// additional settings
//-----------------------------------------------

// weapon types
enum WEAPON {BUGBALL, CASTER};

// facial expression in face pictures
// Stronger feelings has more large integer
enum EMOTION {
	EMOTION_NORMAL, // frame 4
	EMOTION_SURPRISED, // frame 3
	EMOTION_CALM,// frame 8

	EMOTION_LAUGHING, // frame 6
	EMOTION_ANGRY, // frame 7
	EMOTION_FULL_SYNCHRO,// frame 5

	EMOTION_TIRED, // frame 1
	EMOTION_TEARING, // frame 2
	EMOTION_VERY_TIRED // frame 0
};

// the map from facial expressions to frame number in face image
const unsigned int EMOTION_TO_FRAME[] = {
	4,3,8,
	6,7,5,
	1,2,0
};

// movement in casters
enum CASTER_ANIMATION {
	CASTER_UP, // 0000
	CASTER_DOWN, // 0001
	CASTER_RIGHT, // 0010
	CASTER_LEFT, // 0011
	CASTER_NORMAL, // 0100
	CASTER_REST, // 0101
	CASTER_SLEEP, // 0110
	CASTER_MOTION1, // 0111
	CASTER_MOTION2, // 1000
	CASTER_MOTION3, // 1001
	CASTER_MOTION4 // 1010
};


#endif
