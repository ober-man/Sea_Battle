#pragma once
#include <iostream>
#include "Names.h"
#include <vector>
#include <cstdlib>

// Structure for SHIPS data
struct ship 
{
	int x = 0;
	int y = 0;
	bool orient = VERTICAL;
	int type = DECK1;
};

// Structure for coords on board
struct coords 
{
	int x = 0;
	int y = 0;
};

// Game and board
class Board 
{
  private:
    int hit(coords XY);
    int dead_zone(coords XY, int** board);
    int add_hit (coords XY, int** board);
    coords enter_XY();
    int check_inj(coords* XY);
    int create_ship(int** board, int type);
    int Poison_board(int** board);

  public:
    int** Myboard;
    int** Enemyboard;
    int** Hitboard;

    int print(int** board);
    Board();
    int turn(coords XY);
    int add_ship(ship SH, int** board);
    int random_ships(int** board);
      ~Board();
};

// Constuct board
Board::Board() 
{
	this->Myboard = new int* [LSIZE];
	for (int i = 0; i < LSIZE; i++) {
		this->Myboard[i] = new int[LSIZE];
	}
	for (int i = 0; i < LSIZE; i++) {
		for (int j = 0; j < LSIZE; j++) {
			this->Myboard[i][j] = 0;
		}
	}
	Poison_board(this->Myboard);

	this->Enemyboard = new int* [LSIZE];
	for (int i = 0; i < LSIZE; i++) {
		this->Enemyboard[i] = new int[LSIZE];
	}
	for (int i = 0; i < LSIZE; i++) {
		for (int j = 0; j < LSIZE; j++) {
			this->Enemyboard[i][j] = 0;
		}
	}
	Poison_board(this->Enemyboard);

	this->Hitboard = new int* [LSIZE];
	for (int i = 0; i < LSIZE; i++) {
		this->Hitboard[i] = new int[LSIZE];
	}
	for (int i = 0; i < LSIZE; i++) {
		for (int j = 0; j < LSIZE; j++) {
			this->Hitboard[i][j] = 0;
		}
	}
	Poison_board(this->Hitboard);
}

Board::~Board() 
{
	for (int i = 0; i < LSIZE; i++) {
		delete Myboard[i];
	}
    delete [] Myboard;

	for (int i = 0; i < LSIZE; i++) {
		delete Enemyboard[i];
	}
	delete [] Enemyboard;

	for (int i = 0; i < LSIZE; i++) {
		delete Hitboard[i];
	}
	delete [] Hitboard;
}

// Add ship on board
int Board::add_ship(ship SH, int** board) 
{
	// Add vertical ship
	if (SH.orient == HORIZONT) {

		//check size
		if (SH.y + SH.type > SIZE)
			return ERR_PLACE;

		for (int i = 0; i < SH.type; i++)
			board[SH.x][SH.y + i] = SHIP;
	}

	if (SH.orient == VERTICAL) {
		//check size
		if (SH.x + SH.type > SIZE)
			return ERR_PLACE;

		for (int i = 0; i < SH.type; i++)
			board[SH.x + i][SH.y] = SHIP;
	}

	return SUCCESS;
}

// Turn of player
int Board::turn(coords XY) 
{
	if (hit(XY) == HIT) {
		this->Hitboard[XY.x][XY.y] = HIT_SHIP;
		// Add new coords for HIT
		XY = enter_XY();
		// New turn with new coords
		turn(XY);
		return HIT;
	}

	else if (hit(XY) == MISS) {
		this->Hitboard[XY.x][XY.y] = HIT_MISS;
		return MISS;
	}

	else if (hit(XY) == KILL) {
		this->Hitboard[XY.x][XY.y] = HIT_SHIP;
		// Add deadzone to board near ship
		dead_zone(XY, this->Hitboard);
		// Add new coords and go on your turn
		XY = enter_XY();
	  turn(XY);
		return KILL;
	}
}

// Check hit on board
int Board::hit(coords XY) 
{
	if (this->Enemyboard[XY.x][XY.y] == SHIP) {
		return check_inj(&XY);
	}
	else
		return MISS;
}

// Input coords XY
coords Board::enter_XY () 
{
    coords Node;
    std::cin >> Node.x;
    std::cin >> Node.y;
    if(Node.x  > 10 || Node.x < 1 || Node.y  > 10 || Node.y < 1) {
        std::cout << "Wrong coordinates, try again" << std::endl;
        Node = enter_XY();
    }
    return Node;
}

// Add dead zone near dead ship
int Board::dead_zone(coords XY, int** board) 
{
	int orient = 0;
	int i = 1;
	coords XY_ = XY;
	add_hit(XY, board);
	if (board[XY.x + 1][XY.y] == SHIP || board[XY.x - 1][XY.y] == SHIP)
		orient = VERTICAL;
	else
		orient = HORIZONT;

	if (orient == VERTICAL) {
		while (board[XY.x + i][XY.y] == SHIP) {
			XY_.x = XY.x + i;
			add_hit(XY_,board);
			i++;
		}
		i = 1;
		while (board[XY.x - i][XY.y] == SHIP) {
			XY_.x = XY.x - i;
			add_hit(XY_,board);
			i--;
		}
	}
	i = 1;
	if (orient == HORIZONT) {
		while (board[XY.x][XY.y + i] == SHIP) {
			XY_.y = XY.y + i;
			add_hit(XY_,board);
			i++;
		}
		i = 1;
		while (board[XY.x][XY.y - i] == SHIP) {
			XY_.y = XY.y - i;
			add_hit(XY_,board);
			i--;
		}

	}

	return SUCCESS;
}

// Find injured cell and return HIT or KILL
int Board::check_inj(coords* XY) 
{
	int orient = 0;
	int i = 1;

	if (this->Enemyboard[XY->x + 1][XY->y] == SHIP || this->Enemyboard[XY->x - 1][XY->y] == SHIP)
		orient = VERTICAL;
	else
		orient = HORIZONT;

	if (orient == VERTICAL) {
		while (this->Enemyboard[XY->x + i][XY->y] == SHIP) {
			if (this->Enemyboard[XY->x + i][XY->y] && this->Hitboard[XY->x + i][XY->y] == 0)
				return HIT;
			i++;
		}
		while (this->Enemyboard[XY->x - i][XY->y] == SHIP) {
			if (this->Enemyboard[XY->x - i][XY->y] && this->Hitboard[XY->x - i][XY->y] == 0)
				return HIT;
			i--;
		}
	}

	if (orient == HORIZONT) {
		while (this->Enemyboard[XY->x][XY->y + i] == SHIP) {
			if (this->Enemyboard[XY->x][XY->y + i] && this->Hitboard[XY->x][XY->y + i] == 0)
				return HIT;
			i++;
		}
		while (this->Enemyboard[XY->x][XY->y - i] == SHIP) {
			if (this->Enemyboard[XY->x][XY->y - i] && this->Hitboard[XY->x][XY->y - i] == 0)
				return HIT;
			i--;
		}
	}

	return KILL;
}

// Add hit around cell
int Board::add_hit(coords XY, int** board) 
{
	if (board[XY.x + 1][XY.y] != SHIP)
		board[XY.x + 1][XY.y] = DEAD_ZONE;
	if (board[XY.x - 1][XY.y] != SHIP)
		board[XY.x - 1][XY.y] = DEAD_ZONE;
	if (board[XY.x][XY.y - 1] != SHIP)
		board[XY.x][XY.y - 1] = DEAD_ZONE;
	if (board[XY.x][XY.y + 1] != SHIP)
		board[XY.x][XY.y + 1] = DEAD_ZONE;
	if (board[XY.x + 1][XY.y + 1] != SHIP)
		board[XY.x + 1][XY.y + 1] = DEAD_ZONE;
	if (board[XY.x - 1][XY.y + 1] != SHIP)
		board[XY.x - 1][XY.y + 1] = DEAD_ZONE;
	if (board[XY.x + 1][XY.y - 1] != SHIP)
		board[XY.x + 1][XY.y - 1] = DEAD_ZONE;
	if (board[XY.x - 1][XY.y - 1] != SHIP)
		board[XY.x - 1][XY.y - 1] = DEAD_ZONE;
	return SUCCESS;
}

// Randoms ships on board
int Board::random_ships(int** board) 
{
	create_ship(board, DECK1);
	create_ship(board, DECK2);
	create_ship(board, DECK3);
	create_ship(board, DECK4);

	return SUCCESS;
}

// Find all free cells in Myboard
std::vector<coords> Board::find_free_cells(int* size, int** board) 
{
	std::vector<coords> cells;
	int k = 0;
	coords XY;
	//int j = 0; - unused
	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			int elm = board[x][y];
			if (elm == 0) {
				XY.x = x;
				XY.y = y;
				cells.push_back(XY);
				k++;
			}
		}
	}

	*size = k;
	return  cells;
}

// Print board in SHELL
int Board::print(int** board) 
{
	for (int i = 0; i < LSIZE; i++) {
		for (int j = 0; j < LSIZE; j++) {
			std::cout << board[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << '\n';
	return SUCCESS;
}

// Create one type random ships
int Board::create_ship(int** board, int type) 
{

	int size = 0;
	coords random_node;
	ship SH;

	for (int i = 0; i < 5 - type; i++) {
		SH.type = type;
		std::vector<coords> free = find_free_cells(&size, board);
		random_node = free[rand() % size];

		SH.x = random_node.x;
		SH.y = random_node.y;
		int rnd = rand();

		if (random_node.x + type < SIZE && rnd % 2 == 1) {
			SH.orient = VERTICAL;
			if (board[SH.x + type - 1][SH.y] == 0) {
				add_ship(SH, board);
				dead_zone(random_node, board);
			}
			else
				i--;
		}
		else if (random_node.y + type < SIZE && rnd % 2 == 1) {
			SH.orient = HORIZONT;
			if (board[SH.x][SH.y + type - 1] == 0) {
				add_ship(SH,board);
				dead_zone(random_node, board);
			}
			else
				i--;
		}
		else
			i--;
	}
	return SUCCESS;
}

// Create poison for board
int Board::Poison_board(int** board) 
{
	for (int i = 0; i < LSIZE; i++) {
		board[0][i] = POISON;
	}

	for (int i = 0; i < LSIZE; i++) {
		board[SIZE][i] = POISON;
	}

	for (int i = 0; i < LSIZE; i++) {
		board[i][0] = POISON;
	}

	for (int i = 0; i < LSIZE; i++) {
		board[i][SIZE] = POISON;
	}

	return SUCCESS;
}
