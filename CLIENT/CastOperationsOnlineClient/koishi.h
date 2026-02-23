// Cast Operations Online Client
// koishi.h
#pragma once
#ifndef _KOISHI_H
#define _KOISHI_H
#include "caster.h"
class Koishi : public Caster
{
private:
	DISALLOW_COPY_AND_ASSIGN(Koishi);
	float invisibleTimer_; // timer during which Koishi is invisible by spellCard()

public:
	Koishi();
	~Koishi() {};

	// inherited member functions
	//-------------------------------------------
	// use her special skill
	virtual void spellCard();
	virtual void update(float frameTime);

	// escape from drawwing Koishi
	// when she vanishes by spellcard
	void vanishingDraw() {
		reflectEmotion();
		face.draw();
	}

	// any other trivial functions
	//-------------------------------------------
	float getInvisibleTimer() {
		return invisibleTimer_;
	}
};

#endif
