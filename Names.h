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

	// game modes
	COMP_GAME = 1,
	SUPER_FAN = 2,
};

enum ERRORS
{
	SUCCESS = 0,
	ERR_PLACE = -100,
	ERR_MODE = -200,
	ERR_STATUS = -300,
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

// board size
int LSIZE = 12;
int SIZE = 11;
const int len = 39;
const int x_0 = 169;
const int y_0 = 230;
const int x_2 = 1300;
const int y_2 = 230;
const int x_0_fan = 100;
const int y_0_fan = 55;
const int x_2_fan = 1000;
const int y_2_fan = 55;
