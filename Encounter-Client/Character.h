#pragma once
#include "Object.h"
#include <string>
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
	void setSpriteHero(std::string src_);

	friend class Game;
};

