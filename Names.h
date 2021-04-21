#pragma once
enum Consts {
	HORIZONT = 0,
	VERTICAL = 1,
	DECK4 = 4,
	DECK3 = 3,
	DECK2 = 2,
	DECK1 = 1,
	SIZE = 11,
	LSIZE = 12,
	POISON = 2,
	DEAD_ZONE = 3,
	SHIP = 5,
	HIT = 1,
	HIT_SHIP = 5,
	HIT_MISS = 3,
	MISS = 0,
	KILL = 2,
	INJURED = 2,
	NUM_OF_SHIPS = 4
};

enum ERRORS {
	SUCCESS = 0,
	ERR_PLACE = 1,

};

const int len = 38.5;
const int y_cell = 44;
const int grid = 7;
const int x_0 = 1060;
const int y_0 = 169;
const int x_end = 1642;
const int y_end = 666;
const int my_x_0 = 176;
const int my_x_end = 757;
