// Cast Operations Online Server
// koishi.h

#ifndef _KOISHI_H
#define _KOISHI_H
#include "caster.h"

class Koishi : public Caster
{
private:
	DISALLOW_COPY_AND_ASSIGN(Koishi);
	float invisibleTimer_;// timer during which Koishi is invisible by spellCard()

public:
	Koishi();

	// inherited member functions
	//-----------------------------------------
	// use her special skill in a game:w
	virtual void spellCard();
	virtual void update(float frameTime);

	// any other trivial functions
	//-----------------------------------------
	float getInvisibleTimer() {
		return invisibleTimer_;
	}
};

#endif