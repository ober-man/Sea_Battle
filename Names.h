#pragma once
const int ASCII_CODE = 48;

enum Consts
{
	HORIZONT = 0,
	VERTICAL = 1,

	DECK4 = 4,
	DECK3 = 3,
	DECK2 = 2,
	DECK1 = 1,

	LSIZE = 12,
	SIZE = LSIZE - 1,

	POISON = 2,
	DEAD_ZONE = 3,
	SHIP = 5,
	HIT = 1,

	HIT_SHIP = 5,
	HIT_MISS = 3,
	HIT_MYSHIP = 6,

	MISS = 0,
	KILL = 2,
	INJURED = 2,

	PLAYER_WIN = 1,
	PLAYER_LOSE = -1,

	PLAYER_MISS = 6,
	PLAYER_SHOOT_AGAIN = 7,
	COMP_MISS = 8,
	COMP_SHOOT_AGAIN = 9,
};

enum ERRORS
{
	SUCCESS = 0,
	ERR_PLACE = -100,
};

// Struct for SHIPS data
struct ship
{
	int x = 0;
	int y = 0;
	bool orient = VERTICAL;
	int type = DECK1;
};

// Struct for coords on board
struct coords
{
	int x = 0;
	int y = 0;
};

const int len = 39;
const int x_0 = 169;
const int y_0 = 169;
const int x_2 = 1000;
const int y_2 = 169;
