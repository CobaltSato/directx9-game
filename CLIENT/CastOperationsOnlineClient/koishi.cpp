// Cast Operations Online Client
// koishi.cpp
#include "koishi.h"
Koishi::Koishi() {
	invisibleTimer_ = 0.0f;
}

void Koishi::spellCard() {
	if (hasCard_ == true) {
		invisibleTimer_ = 20.0f;
	}
	hasCard_ = false;
}

void Koishi::update(float frameTime) {
	if (invisibleTimer_ > 0) {
		invisibleTimer_ -= frameTime;
		//face.setCurrentFrame(6);
	}
	Caster::update(frameTime);
}
