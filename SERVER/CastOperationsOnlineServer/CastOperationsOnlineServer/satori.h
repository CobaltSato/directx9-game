// Cast Operations Online Server
// satori.h

#ifndef _SATORI_H
#define _SATORI_H
#include "caster.h"
class Satori : public Caster
{
private:
	DISALLOW_COPY_AND_ASSIGN(Satori);
public:
	Satori();
	// inherited member functions
	//-----------------------------------------
	// use her special skill in a game
	virtual void spellCard();
	virtual void update(float frameTime);
};

#endif
