// Cast Operations Online Server
// satori.cpp

#include "satori.h"
Satori::Satori() 
{
}
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