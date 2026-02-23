// Cast Operations Online Server
// koishi.cpp

#include "koishi.h"
Koishi::Koishi() {
	invisibleTimer_ = 0;
}

void Koishi::spellCard() {
	if (hasCard_) {
		invisibleTimer_ = 20.0f;
	}
	hasCard_ = false;
}

void Koishi::update(float frameTime) {
	if (invisibleTimer_ > 0)
		invisibleTimer_ -= frameTime;
	Caster::update(frameTime);
}
