// Cast Operations Online Client
// conversation.cpp

#include "conversation.h"

//=============================================================================
//  constructor
//=============================================================================
Conversation::Conversation()
:hasRead(false)
{
	fileHandle = new FileHandle(conversationNS::DEFAULT_MESSAGE);
}

bool Conversation::initializeConversation(
	Graphics* graphics,
	TextureManager* pConversationTextures,
	TextureManager* pSatoriStandingTextures,
	TextureManager* pKoishiStandingTextures
)
{
	// initialize DirectX fonts
	fontSmall.initialize(graphics, conversationNS::FONT_SIZE, false, false, conversationNS::FONT);
	fontSmall.setFontColor(conversationNS::MESSENGER_COLOR);
	read();

	bool messageBoxInitialized = initialize(graphics, conversationNS::WIDTH, conversationNS::HEIGHT,
		conversationNS::COLUMNS, pConversationTextures);
	setFrames(conversationNS::SATORI_FRAME, conversationNS::SATORI_FRAME);
	setCurrentFrame(conversationNS::SATORI_FRAME);
	setX(conversationNS::X);
	setY(conversationNS::Y);
	bool satoriStandingInitialized = satoriStanding.initialize(graphics, conversationNS::STANDING_WIDTH, conversationNS::STANDING_HEIGHT,
		conversationNS::STANDING_TEXTURE_COLS, pSatoriStandingTextures);
	satoriStanding.setX(conversationNS::SATORI_X);
	satoriStanding.setY(conversationNS::SATORI_Y);
	satoriStanding.setCurrentFrame(3);
	bool koishiStandingInitialized = koishiStanding.initialize(graphics, conversationNS::STANDING_WIDTH, conversationNS::STANDING_HEIGHT,
		conversationNS::STANDING_TEXTURE_COLS, pKoishiStandingTextures);
	koishiStanding.setX(conversationNS::KOISHI_X);
	koishiStanding.setY(conversationNS::KOISHI_Y);
	koishiStanding.setCurrentFrame(3);
	return messageBoxInitialized && satoriStandingInitialized && koishiStandingInitialized;
}

Conversation::~Conversation() {
	SAFE_DELETE(fileHandle);
}

void Conversation::read() {
	speaker = fileHandle->readLine();
	state = fileHandle->readLine();
	for (int i = 0; i < conversationNS::MESSAGE_LENGTH; ++i) {
		message[i] = fileHandle->readLine();
	}
}

void Conversation::write() {
	// assert speaker is not empty
	if (speaker.length() != 0) {
		if (speaker.compare("END") == 0) {
			hasRead = true;
			return;
		}
		if (speaker.compare("satori") == 0) {
			setCurrentFrame(conversationNS::SATORI_FRAME);
			satoriStanding.setCurrentFrame(getState());
		}
		else {
			setCurrentFrame(conversationNS::KOISHI_FRAME);
			koishiStanding.setCurrentFrame(getState());
		}
		koishiStanding.draw();
		satoriStanding.draw();
		draw(graphicsNS::ALPHA75);
		for (int i = 0; i < conversationNS::MESSAGE_LENGTH; ++i) {
			fontSmall.print(message[i],
				conversationNS::MESSAGE_X,
				conversationNS::MESSAGE_Y + conversationNS::LINE_HEIGHT*i);
		}
	}
}

// refer to pictures/satoriStanding.png, pictures/koishiStanding.png
StandingState Conversation::getState() {
	// row 1
	if (state.compare("NORMAL_TEARING") == 0)
		return NORMAL_TEARING;
	if (state.compare("NORMAL_ANGRY") == 0)
		return NORMAL_ANGRY;
	if (state.compare("NORMAL_SURPRISED") == 0)
		return NORMAL_SURPRISED;
	if (state.compare("NORMAL_WORRIED") == 0)
		return NORMAL_WORRIED;
	if (state.compare("NORMAL_LAUGHING") == 0)
		return NORMAL_LAUGHING;
	if (state.compare("NORMAL_ANGRY") == 0)
		return NORMAL_ANGRY;
	if (state.compare("NORMAL_SMILE") == 0)
		return NORMAL_SMILE;
	if (state.compare("NORMAL_DISCONTENTED") == 0)
		return NORMAL_DISCONTENTED;
	if (state.compare("NORMAL_EASY") == 0)
		return NORMAL_EASY;
	if (state.compare("NORMAL_SHADED") == 0)
		return NORMAL_SHADED;

	// row 2
	if (state.compare("MORE_TEARING") == 0)
		return MORE_TEARING;
	if (state.compare("MORE_SURPRISED") == 0)
		return MORE_SURPRISED;
	if (state.compare("MORE_WORRIED") == 0)
		return MORE_WORRIED;
	if (state.compare("MORE_LAUGHING") == 0)
		return MORE_LAUGHING;
	if (state.compare("MORE_ANGRY") == 0)
		return MORE_ANGRY;
	if (state.compare("MORE_DISCONTENTED") == 0)
		return MORE_DISCONTENTED;
	if (state.compare("MORE_EASY") == 0)
		return MORE_EASY;

	// row 3
	if (state.compare("LOSE_TEARING") == 0)
		return LOSE_TEARING;
	if (state.compare("LOSE_SUPRISED") == 0)
		return LOSE_SUPRISED;
	if (state.compare("LOSE_SMILE") == 0)
		return LOSE_SMILE;
	if (state.compare("LOSE_LAUGHING") == 0)
		return LOSE_LAUGHING;
	if (state.compare("LOSE_ANGRY") == 0)
		return LOSE_ANGRY;
	if (state.compare("LOSE_DISCONTENTED") == 0)
		return LOSE_SUPRISED;
	if (state.compare("LOSE_EASY") == 0)
		return LOSE_SUPRISED;
	if (state.compare("LOSE_SHADED") == 0)
		return LOSE_SUPRISED;
	if (state.compare("LOSE_SUPRISED") == 0)
		return LOSE_SUPRISED;

	// row 4
	if (state.compare("LOSE_MORE_TEARING") == 0)
		return LOSE_MORE_TEARING;
	if (state.compare("LOSE_MORE_SUPRISED") == 0)
		return LOSE_MORE_SUPRISED;
	if (state.compare("LOSE_MORE_WORRIED") == 0)
		return LOSE_MORE_WORRIED;
	if (state.compare("LOSE_MORE_LAUGHING") == 0)
		return LOSE_MORE_LAUGHING;
	if (state.compare("LOSE_MORE_DISCONTENTED") == 0)
		return LOSE_MORE_TEARING;
	if (state.compare("LOSE_MORE_EASY") == 0)
		return LOSE_MORE_EASY;

	// otherwise
	return NORMAL_SMILE;
}

