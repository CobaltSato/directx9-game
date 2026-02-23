// Cast Operations Online Server
// constants.h
#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

// Useful macros
//-----------------------------------------------
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
//-----------------------------------------------
// window setting
const char CLASS_NAME[] = "Cast Operations Online";
const char GAME_TITLE[] = "Cast Operations Online Server ";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels
 
const double PI = 3.14159265358979;
const double PIx2 = PI*2.0;
const float FRAME_RATE = 200.0f;            // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float FULL_HEALTH = 1800;
const float BUG_BALL_SPEED = 20;

// graphic images
//--------------------------------------------------------------------------------
const char NEBULA_IMAGE[] =   "pictures\\orion.jpg";     // photo source NASA/courtesy of nasaimages.org 
const char TEXTURES_IMAGE[] = "pictures\\textures.png";  // game textures
const char MENU_IMAGE[] =     "pictures\\menu.png";      // menu texture
const char SATORI_IMAGE[] =     "pictures\\satori.png";      // menu texture
const char KOISHI_IMAGE[] =     "pictures\\koishi.png";      // menu texture
const char BUGBALL_IMAGE[] =     "pictures\\bugBall.png";      // menu texture
const char MOON_IMAGE[] =     "pictures\\moon.png";      // menu texture

// audio 
//--------------------------------------------------------------------------------
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "";

// key mappings
//--------------------------------------------------------------------------------
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY  = VK_OEM_3;    // `~ key for U.S.
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key

// constants for 3 cross 6 tiles board 
const int BOARD_FRAME = 0;
const int BOARD_WIDTH = 6;
const int BOARD_HEIGHT = 3;
const int BOARD_TEXTURE_SIZE = 64;
const int BOARD_TEXTURE_COLS = 2;
const int BOARD_X = GAME_WIDTH / 2 - BOARD_TEXTURE_SIZE * BOARD_WIDTH / 2;
const int BOARD_X_MAX = BOARD_X + BOARD_WIDTH * BOARD_TEXTURE_SIZE;
const int BOARD_Y = GAME_HEIGHT / 2;
const int BOARD_Y_MAX = GAME_HEIGHT / 2 + BOARD_HEIGHT * BOARD_TEXTURE_SIZE /2;


// weapon types for damage
enum WEAPON {BUGBALL,  MOON};

// current movement of a caster 
enum CASTER_ANIMATION {
	CASTER_UP,
	CASTER_DOWN,
	CASTER_RIGHT,
	CASTER_LEFT, 	
	CASTER_NORMAL, 	
	CASTER_REST,
	CASTER_SLEEP,
	CASTER_MOTION1, 
	CASTER_MOTION2, 
	CASTER_MOTION3,
	CASTER_MOTION4
};

// facial expression in face pictures
// Stronger feelings has more large integer
enum EMOTION {
	EMOTION_NORMAL,
	EMOTION_SURPRISED,
	EMOTION_CALM,

	EMOTION_LAUGHING,
	EMOTION_ANGRY,
	EMOTION_FULL_SYNCHRO,

	EMOTION_TIRED,
	EMOTION_TEARING,
	EMOTION_VERY_TIRED
};

// satori data setting
namespace satoriConstantsNS {
	// dot picture
	const int DOT_WIDTH = 32;
	const int DOT_HEIGHT = 32;
	const int DOT_X = GAME_WIDTH / 4 - DOT_WIDTH;
	const int DOT_Y = GAME_HEIGHT / 2 - DOT_HEIGHT + 60;
	const int DOT_NCOLS = 3;
	const int DOT_START_FRAME = 12;
	const int DOT_END_FRAME = 14;
};
// koishi data setting
namespace koishiConstantsNS {
	// dot picture
	const int DOT_WIDTH = 32;
	const int DOT_HEIGHT = 32;
	const int DOT_X = GAME_WIDTH - GAME_WIDTH / 4;
	const int DOT_Y = GAME_HEIGHT / 2 + 60;
	const int DOT_NCOLS = 3;
	const int DOT_DEFAULT_FRAME = 12;
	const int DOT_START_FRAME = 12;
	const int DOT_END_FRAME = 14;

};

#endif