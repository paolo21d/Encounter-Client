#pragma once
#include "Object.h"
class Character : public Object
{
	int strength;
	int intelligence;
	int vitality;
	int gold;
public:
	Character();
	~Character();
};

