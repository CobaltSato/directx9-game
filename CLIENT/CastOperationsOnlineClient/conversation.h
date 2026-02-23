#pragma once
// Cast Operations Online Client
// conversation.h 

#ifndef _MESSAGE_BOX_HANDLER_H               // Prevent multiple definitions if this 
#define _MESSAGE_BOX_HANDLER_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "textureManager.h"
#include "constants.h"
#include "FileHandle.h"
#include "textDX.h"
#include "graphics.h"
#include "gameError.h"
#include <vector>
#include <string>
#include <istream>
#include <iterator>
namespace conversationNS
{
    const int   WIDTH = 500;                // image width
    const int   HEIGHT = 100;               // image height
    const int   X = GAME_WIDTH/2 - WIDTH/2; // centered in x axis
    const int   Y = GAME_HEIGHT - HEIGHT; // set lower in y axis 
    const int   TEXTURE_COLS = 1;       // texture has 2 columns
    const int   SATORI_FRAME = 0;        // starts at frame 1
    const int   KOISHI_FRAME = 1;          // no animation
	const int	FONT_SIZE = 20; 
	const int	COLUMNS = 1;

    const int   STANDING_WIDTH = 200;                // image width
    const int   STANDING_HEIGHT = 400;               // image height
    const int   STANDING_TEXTURE_COLS = 9;       // texture has 2 columns

    const int   SATORI_X = 70; // centered in x axis
    const int   SATORI_Y = 200; // set lower in y axis 
    const int   KOISHI_X = GAME_WIDTH - STANDING_WIDTH -70; // centered in x axis
    const int   KOISHI_Y = 170; // set lower in y axis 

    const char FONT[] = "Arial Bold";  // font
	const std::string DEFAULT_MESSAGE =     "text\\message.txt";      // textfile
	const std::string SATORI_MESSAGE =     "text\\satoriMessage.txt";      // textfile
	const std::string KOISHI_MESSAGE =     "text\\koishiMessage.txt";      // textfile
	const int MESSAGE_LENGTH = 3;
	const int MESSAGE_X = GAME_WIDTH/4;
	const int MESSAGE_Y = GAME_HEIGHT - 72;
	const int LINE_HEIGHT = 24;
    const COLOR_ARGB MESSENGER_COLOR = graphicsNS::WHITE;
}
enum StandingState {
	// row 1
	NORMAL_TEARING,
	NORMAL_SURPRISED,
	NORMAL_WORRIED,
	NORMAL_LAUGHING,
	NORMAL_ANGRY,
	NORMAL_SMILE,
	NORMAL_DISCONTENTED,
	NORMAL_EASY,
	NORMAL_SHADED,
	// row 2
	MORE_TEARING,
	MORE_SURPRISED,
	MORE_WORRIED,
	MORE_LAUGHING,
	MORE_ANGRY,
	MORE_DISCONTENTED,
	MORE_EASY,
	NONE1, // padding
	NONE2, // padding
	// row 3
	LOSE_TEARING,
	LOSE_SUPRISED,
	LOSE_SMILE,
	LOSE_WORRIED,
	LOSE_LAUGHING,
	LOSE_ANGRY,
	LOSE_DISCONTENTED,
	LOSE_EASY,
	LOSE_SHADED,
	// row 4
	LOSE_MORE_TEARING,
	LOSE_MORE_SUPRISED,
	LOSE_MORE_WORRIED,
	LOSE_MORE_LAUGHING,
	LOSE_MORE_ANGRY,
	LOSE_MORE_DISCONTENTED,
	LOSE_MORE_EASY,
	NONE3,
	NONE4
};

class Conversation : public Image            // inherits from Entity class
{
private:
	DISALLOW_COPY_AND_ASSIGN(Conversation);
	TextDX	fontSmall;
	Image satoriStanding, koishiStanding;
	std::string speaker; // current messages
	std::string state; // current messages
	std::string message[conversationNS::MESSAGE_LENGTH]; // current messages
	FileHandle* fileHandle; // text file object for messages 
	bool hasRead;

public:
	Conversation();
	~Conversation();
	//bool initializeConversation(Graphics *graphics,TextureManager* conversationTextures,);
	bool initializeConversation(Graphics *graphics,
		TextureManager* pConversationTextures,
		TextureManager* pSatoriStandingTextures,
		TextureManager* pKoishiStandingTextures
	);
	void read();
	void write();
	bool getHasRead() {
		return hasRead;
	}
	void satoriWon() {
		for (int i = 0; i < 3; ++i) {
			message[i] = "";
		}
		fileHandle->branch(conversationNS::SATORI_MESSAGE);
	}
	void koishiWon() {
		for (int i = 0; i < 3; ++i) {
			message[i] = "";
		}
		fileHandle->branch(conversationNS::KOISHI_MESSAGE);
	}
	TextDX* getFontSmall() { return &fontSmall; }
	StandingState getState();
};
#endif

