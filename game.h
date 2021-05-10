#pragma once
#include"board.h"
#include <chrono>
#include <thread>

// Game and board
class Player
{
    private:
        int kills;
        int enemy_kills;
        int ship_number;
        coords Last_hit;
        int orient_last_hit;
        coords target(coords XY, int &orient);
        int hit(coords XY, Board& board, Board& board2);
        int check_inj(coords XY, Board& board1, Board& board2);
        int extra_hit(coords XY, Board& B1, Board& B2);

    public:
        Board* Myboard;
        Board* Enemyboard;
        Board* Hitboard;
        Board* HitEnemyboard;

        Player();
        ~Player();
        int Get_kills() const;
        int Get_enemy_kills() const;
        int turn(coords XY);
        int enemy_turn(); // private?
        int extra_turn(coords XY);
        int extra_enemy_turn();
        int random_position();
        coords enter_XY();
};

// Construct board
Player::Player()
{
    kills = 0;
	orient_last_hit = NOTHING;
	Last_hit.x = 0;
	Last_hit.y = 0;
	enemy_kills = 0;
	ship_number = 10;
    Myboard = new Board;
	Enemyboard = new Board;
	Hitboard = new Board;
	HitEnemyboard = new Board;
}

Player::~Player()
{
    delete Myboard;
	delete Enemyboard;
	delete Hitboard;
	delete HitEnemyboard;
}

int Player::Get_kills() const
{
    return kills;
}

int Player::Get_enemy_kills() const
{
    return enemy_kills;
}

// Create random position
int Player::random_position()
{
    Myboard->random_ships();
    Enemyboard->random_ships();
    return SUCCESS;
}

coords Player::target(coords XY, int &orient)
{
	if (orient == NOTHING) {
		if (this->HitEnemyboard->Get(XY.x, XY.y + 1) == 0) {
			XY.y++;
			if (this->Myboard->Get(XY.x, XY.y) == SHIP)
				orient = HORIZONT;
		}
		else if (this->HitEnemyboard->Get(XY.x, XY.y - 1) == 0) {
			XY.y--;
			if (this->Myboard->Get(XY.x, XY.y) == SHIP)
				orient = HORIZONT;
		}
		else if (this->HitEnemyboard->Get(XY.x + 1, XY.y) == 0) {
			XY.x++;
			if (this->Myboard->Get(XY.x, XY.y) == SHIP)
				orient = VERTICAL;
		}
		else if (this->HitEnemyboard->Get(XY.x - 1, XY.y) == 0) {
			XY.x--;
			if (this->Myboard->Get(XY.x, XY.y) == SHIP)
				orient = VERTICAL;
		}
	}

	else if (orient == VERTICAL) {
		if (this->HitEnemyboard->Get(XY.x + 1, XY.y) == 0)
			XY.x++;
		else if (this->HitEnemyboard->Get(XY.x - 1, XY.y) == 0)
			XY.x--;
	}

	else if (orient == HORIZONT) {
		if (this->HitEnemyboard->Get(XY.x, XY.y + 1) == 0)
			XY.y++;
		else if (this->HitEnemyboard->Get(XY.x, XY.y - 1) == 0)
			XY.y--;
	}

	return XY;
}

// Turn of computer
int Player::enemy_turn()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	coords XY;

	if (Last_hit.x != 0 && Last_hit.y != 0) {
		XY = target(Last_hit,this->orient_last_hit);
	}
	else {
		int size = 0;
		std::vector<coords> free = HitEnemyboard->free_cells(&size);
		XY = free[rand() % size];
	}

	if(hit(XY, *Myboard, *HitEnemyboard) == HIT)
    {
		HitEnemyboard->Set(XY.x, XY.y, HIT_SHIP);
		this->Last_hit = XY;
		// If hit, computer can continue shooting
		return COMP_SHOOT_AGAIN;
	}
	else if(hit(XY, *Myboard, *HitEnemyboard) == MISS)
    {
		HitEnemyboard->Set(XY.x, XY.y, HIT_MISS);
		// If computer miss - player shoots next
		return COMP_MISS;
	}
	else if(hit(XY, *Myboard, *HitEnemyboard) == KILL)
	{
		HitEnemyboard->Set(XY.x, XY.y, HIT_SHIP);
		// Add dead zone around ship
		HitEnemyboard->dead_zone(XY, HIT_SHIP);
		++enemy_kills;
        // If hit, computer can continue shooting
        // if computer haven't won yet
		this->Last_hit.x = 0;
		this->Last_hit.y = 0;
		this->orient_last_hit = NOTHING;
		if (enemy_kills < ship_number)
			return COMP_SHOOT_AGAIN;
		else return PLAYER_LOSE;
	}
    else return ERR_PLACE;
}

// Turn of player
int Player::turn(coords XY)
{
	if(hit(XY, *Enemyboard, *Hitboard) == HIT)
    {
		Hitboard->Set(XY.x, XY.y, HIT_SHIP);
		// If hit, you can continue shooting
		return PLAYER_SHOOT_AGAIN;
	}
	else if(hit(XY, *Enemyboard, *Hitboard) == MISS)
    {
		Hitboard->Set(XY.x, XY.y, HIT_MISS);
		// If miss - your opponent shoots next
		return PLAYER_MISS;
	}
	else if(hit(XY, *Enemyboard, *Hitboard) == KILL)
    {
		Hitboard->Set(XY.x, XY.y, HIT_SHIP);
		// Add dead zone around ship
		Hitboard->dead_zone(XY, HIT_SHIP);
		++kills;
        // If hit, you can continue shooting
        // if you haven't won yet
		if(kills < ship_number)
            return PLAYER_SHOOT_AGAIN;
        else return PLAYER_WIN;
	}
	else return ERR_PLACE;
}

// Check hit on board
int Player::hit(coords XY, Board& board, Board& board2)
{

	if (board.Get(XY.x, XY.y) == SHIP)
		return check_inj(XY, board, board2);
	else
		return MISS;
}

// Input coords XY
coords Player::enter_XY()
{
	coords Node;
	char x = 0;
	int y = 0;
	// Cycle for input success coords
	while(true)
    {
		Node.x = 0;
		Node.y = 0;

		std::cout << "Please, input coords (Like a A1, B2, E9 and etc) :" << std::endl;
		std::cin >> x;
		std::cin >> y;

		switch (x) {
		case ('A'):
			Node.y = 1;
			break;
		case ('B'):
			Node.y = 2;
			break;
		case ('C'):
			Node.y = 3;
			break;
		case ('D'):
			Node.y = 4;
			break;
		case ('E'):
			Node.y = 4;
			break;
		case ('F'):
			Node.y = 5;
			break;
		case ('G'):
			Node.y = 6;
			break;
		case ('H'):
			Node.y = 7;
			break;
		case ('I'):
			Node.y = 8;
			break;
		case ('J'):
			Node.y = 9;
			break;
		case ('K'):
			Node.y = 10;
			break;
		}
		if (y > 0 && y <= 10)
			Node.x = y;

		if (Node.x != 0 && Node.y != 0) {
			std::cout << "Accept your coords!" << std::endl;
			break;
		}
		else
			std::cout << "Your coords is WRONG!!!! Please repeat!" << std::endl;

	}
	return Node;
}

// Find injured cell and return HIT or KILL
int Player::check_inj(coords XY, Board& board1, Board& board2)
{
	int orient = 0;
	int i = 1;

	if (board1.Get(XY.x + 1, XY.y) == SHIP || board2.Get(XY.x - 1, XY.y) == SHIP)
		orient = VERTICAL;
	else
		orient = HORIZONT;

	if (orient == VERTICAL)
	{
		while (board1.Get(XY.x + i, XY.y) == SHIP)
		{
			if (board1.Get(XY.x + i, XY.y) && board2.Get(XY.x + i, XY.y) == 0)
				return HIT;
			i++;
		}
		while (board1.Get(XY.x - i, XY.y) == SHIP)
		{
			if (board1.Get(XY.x - i, XY.y) && board2.Get(XY.x - i, XY.y) == 0)
				return HIT;
			i--;
		}
	}

	if (orient == HORIZONT)
	{
		while (board1.Get(XY.x, XY.y + i) == SHIP)
		{
			if (board1.Get(XY.x, XY.y + i) && board2.Get(XY.x, XY.y + i) == 0)
				return HIT;
			i++;
		}
		while (board1.Get(XY.x, XY.y - i) == SHIP)
		{
			if (board1.Get(XY.x, XY.y - i) && board2.Get(XY.x, XY.y - i) == 0)
				return HIT;
			i--;
		}
	}

	return KILL;
}

// EXTRA MODE
int Player::extra_turn(coords XY)
{
	kills += extra_hit(XY,*Enemyboard, *Hitboard);
	Hitboard->add_extra_hit(XY);

	if (kills < ship_number)
		return PLAYER_MISS;
	else return PLAYER_WIN;

	return ERR_PLACE;
}

int Player::extra_enemy_turn()
{

	int size = 0;
	std::vector<coords> free = HitEnemyboard->free_cells(&size);
	coords XY = free[rand() % size];

	enemy_kills += extra_hit(XY,*Myboard, *HitEnemyboard);
	HitEnemyboard->add_extra_hit(XY);

	if (enemy_kills < ship_number)
		return PLAYER_MISS;
	else return PLAYER_WIN;

	return ERR_PLACE;
}

int Player::extra_hit(coords XY, Board& B1, Board& B2)
{
	int dead_count = 0;
	if (B1.Get(XY.x, XY.y) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	if (B1.Get(XY.x, XY.y + 1) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	if (B1.Get(XY.x, XY.y - 1) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	if (B1.Get(XY.x + 1, XY.y) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	if (B1.Get(XY.x + 1, XY.y + 1) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	if (B1.Get(XY.x + 1, XY.y - 1) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	if (B1.Get(XY.x - 1, XY.y) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	if (B1.Get(XY.x - 1, XY.y + 1) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	if (B1.Get(XY.x - 1, XY.y - 1) == SHIP)
		if (check_inj(XY, B1, B2) == KILL) {
			dead_count++;
			B2.dead_zone(XY, SHIP);
		}
	return dead_count;
}
