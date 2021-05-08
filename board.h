#pragma once
#include<iostream>
#include <vector>
#include"Names.h"

class Board
{
    private:
        int** board;

        int Poison_board();
        int create_ship(int type);
        int add_ship(ship SH);
        int add_hit(coords XY, int check);
        int clean_dead_zone();

    public:
        Board();
        ~Board();
        void Set(int i, int j, int num);
        int Get(int i, int j) const;
        std::vector<coords> free_cells(int* size) const;
        int dead_zone(coords XY, int check);            // private?
        int random_ships();                             // private?
        void Print() const;
};

// Create board
Board::Board()
{
    board = new int* [LSIZE];
	for(int i = 0; i < LSIZE; i++)
		board[i] = new int[LSIZE];

    for (int i = 0; i < LSIZE; i++)
		for (int j = 0; j < LSIZE; j++)
			board[i][j] = 0;
    Poison_board();
}

// Create poison for board
int Board::Poison_board()
{
	for (int i = 0; i < LSIZE; i++)
		Set(0, i, POISON);
	for (int i = 0; i < LSIZE; i++)
		Set(SIZE, i, POISON);
	for (int i = 0; i < LSIZE; i++)
		Set(i, 0, POISON);
	for (int i = 0; i < LSIZE; i++)
		Set(i, SIZE, POISON);
	return SUCCESS;
}

Board::~Board()
{
	for(int i = 0; i < LSIZE; i++)
		delete board[i];
    delete [] board;
}

void Board::Set(int i, int j, int num)
{
    board[i][j] = num;
}

int Board::Get(int i, int j) const
{
    return board[i][j];
}

void Board::Print() const
{
    std::cout << '\n';
	for (int i = 0; i < LSIZE; i++)
    {
		for (int j = 0; j < LSIZE; j++)
			std::cout << board[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << '\n';
}

// Randoms ships on board
int Board::random_ships()
{
	create_ship(DECK4);
	create_ship(DECK3);
	create_ship(DECK2);
	create_ship(DECK1);
	clean_dead_zone();
	return SUCCESS;
}

// Find all free cells in board
std::vector<coords> Board::free_cells(int* size) const
{
	std::vector<coords> cells;
	int k = 0;
	coords XY;
	for(int x = 0; x < SIZE; x++)
		for(int y = 0; y < SIZE; y++)
			if(Get(x, y) == 0)
            {
				XY.x = x;
				XY.y = y;
				cells.push_back(XY);
				k++;
			}
	*size = k;
	return cells;
}

// Create one type random ships
int Board::create_ship(int type)
{
	int size = 0;
	coords random_node;
	ship SH;

	for(int i = 0; i < 5 - type; i++)
    {
		SH.type = type;
		std::vector<coords> free = free_cells(&size);
		random_node = free[rand() % size];

		SH.x = random_node.x;
		SH.y = random_node.y;

		if(random_node.x + type < SIZE && rand() % 2 == 1)
        {
			SH.orient = VERTICAL;
			if(Get(SH.x + type - 1, SH.y) == 0)
			{
				add_ship(SH);
				dead_zone(random_node, SHIP);
			}
			else
				i--;
		}
		else if(random_node.y + type < SIZE && rand() % 2 == 1)
		{
			SH.orient = HORIZONT;
			if(Get(SH.x, SH.y + type - 1) == 0)
			{
				add_ship(SH);
				dead_zone(random_node, SHIP);
			}
			else
				i--;
		}
		else
			i--;
	}
	return SUCCESS;
}

// Add ship on board
int Board::add_ship(ship SH)
{
	// Add vertical ship
	if(SH.orient == HORIZONT)
    {
		//check size
		if(SH.y + SH.type > SIZE)
			return ERR_PLACE;

		for(int i = 0; i < SH.type; i++)
			Set(SH.x, SH.y + i, SHIP);
	}
	else if(SH.orient == VERTICAL)
    {
		//check size
		if (SH.x + SH.type > SIZE)
			return ERR_PLACE;

		for(int i = 0; i < SH.type; i++)
			Set(SH.x + i, SH.y, SHIP);
	}

	return SUCCESS;
}

//Copy only ships on board
int Board::clean_dead_zone()
{
	for(int i = 0; i < LSIZE; i++)
		for(int j = 0; j < LSIZE; j++)
			if(Get(i, j) != (SHIP || POISON))
				Set(i, j, 0);

	return SUCCESS;
}

// Add dead zone near dead ship
int Board::dead_zone(coords XY, int check)
{
	int orient = 0;
	int i = 1;
	coords XY_ = XY;
	add_hit(XY, check);

	if(Get(XY.x + 1, XY.y) == check || Get(XY.x - 1, XY.y) == check)
		orient = VERTICAL;
	else
		orient = HORIZONT;

	if(orient == VERTICAL)
    {
		while(Get(XY.x + i, XY.y) == check)
		{
			XY_.x = XY.x + i;
			add_hit(XY_, check);
			i++;
		}
		i = 1;
		while(Get(XY.x - i, XY.y) == check)
        {
			XY_.x = XY.x - i;
			add_hit(XY_, check);
			i--;
		}
	}
	i = 1;

	if(orient == HORIZONT)
    {
		while(Get(XY.x, XY.y + i) == check)
		{
			XY_.y = XY.y + i;
			add_hit(XY_, check);
			i++;
		}
		i = 1;
		while(Get(XY.x, XY.y - i) == check)
        {
			XY_.y = XY.y - i;
			add_hit(XY_, check);
			i--;
		}
	}

	return SUCCESS;
}

// Add hit around cell
int Board::add_hit(coords XY, int check)
{
	if (Get(XY.x + 1, XY.y) != check)
		Set(XY.x + 1, XY.y, DEAD_ZONE);
	if (Get(XY.x - 1, XY.y) != check)
		Set(XY.x - 1, XY.y, DEAD_ZONE);
	if (Get(XY.x, XY.y - 1) != check)
		Set(XY.x, XY.y - 1, DEAD_ZONE);
	if (Get(XY.x, XY.y + 1) != check)
		Set(XY.x, XY.y + 1, DEAD_ZONE);
	if (Get(XY.x + 1, XY.y + 1) != check)
		Set(XY.x + 1, XY.y + 1, DEAD_ZONE);
	if (Get(XY.x - 1, XY.y + 1) != check)
		Set(XY.x - 1, XY.y + 1, DEAD_ZONE);
	if (Get(XY.x + 1, XY.y - 1) != check)
		Set(XY.x + 1, XY.y - 1, DEAD_ZONE);
	if (Get(XY.x - 1, XY.y - 1) != check)
		Set(XY.x - 1, XY.y - 1, DEAD_ZONE);
	return SUCCESS;
}
