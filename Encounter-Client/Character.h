#pragma once
#include "Object.h"
class Character : public Object
{
public:
	int strength;
	int intelligence;
	int vitality;
	int gold;
	int hp;

	Character();
	~Character();
};

