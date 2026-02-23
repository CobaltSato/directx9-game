// Cast Operations Online Client
// satori.h
#pragma once
#ifndef _SATORI_H
#define _SATORI_H
#include "caster.h"

class Satori : public Caster
{
private:
	DISALLOW_COPY_AND_ASSIGN(Satori);
public:
	Satori() {};
	~Satori() {};
	// inherited member functions
	//-----------------------------------------
	// use her special skill in a game
	virtual void spellCard();
	virtual void update(float frameTime);
};

#endif
