// Cast Operations Online Client
// satori.cpp
#include "satori.h"

void Satori::spellCard() {
	if (hasCard_ == true) {
		shieldTime = 12.0f;
	}
	hasCard_ = false;
}
void Satori::update(float frameTime) {
	if (shieldTime > 0) {
		setShieldOn(true);
		shieldTime -= frameTime;
	}
	else {
		setShieldOn(false);
	}
	Caster::update(frameTime);
}