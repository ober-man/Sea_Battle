#pragma once

enum Consts
{
    // orientation types
	HORIZONT = 0,
	VERTICAL = 1,

	// ship types
	DECK4 = 4,
	DECK3 = 3,
	DECK2 = 2,
	DECK1 = 1,

	// board size
	LSIZE = 12,
	SIZE = LSIZE - 1,

	// cells on Myboard / Enemyboard
	// default = 0
	POISON = 2,
	DEAD_ZONE = 3,
	SHIP = 5,
	HIT = 1,

	// cells on Hitboard / Enemyhitboard
	// default = 0
	HIT_SHIP = 5,
	HIT_MISS = 3,
	//HIT_MYSHIP = 6,

	// status of shooting
	MISS = 0,
	KILL = 2,
	INJURED = 2,

	// status variants:
	// comparing with 0
	// to understand next action
	PLAYER_WIN = 1,
	PLAYER_SHOOT_AGAIN = 2,
	COMP_MISS = 3,
	PLAYER_LOSE = -1,
	PLAYER_MISS = -2,
	COMP_SHOOT_AGAIN = -3,

	NOTHING = -100,
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
const int y_0 = 230;
const int x_2 = 1300;
const int y_2 = 230;
const int x_end = 1642;
const int y_end = 666;
const int my_x_0 = 176;
const int my_x_end = 757;
