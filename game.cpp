#include "game.h"
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

// return number of user kills
int Player::Get_kills() const
{
	return kills;
}

// return number of computer kills
int Player::Get_enemy_kills() const
{
	return enemy_kills;
}

// function for user's ship placement
// if there is enough ships
// it will return 10 = number of ships
int Player::check_ship_number()
{
    int counter = 0;
    for(int i = 0; i < LSIZE; ++i)
        for(int j = 0; j < LSIZE; ++j)
            if(Myboard->Get(i, j) == SHIP)
                counter++;
    if(counter == 20)
    {
        ship_number = 10;
        return ship_number;
    }
    return counter;
}

// Create random position
int Player::random_position()
{
	//srand(time(0));
	Myboard->random_ships();
	Enemyboard->random_ships();
	return SUCCESS;
}

// if computer's last turn was successful
// it will make a target near last hit
coords Player::target(coords XY, int &orient)
{
	if (orient == NOTHING)
    	{
		if (this->HitEnemyboard->Get(XY.x, XY.y + 1) == 0) {
			XY.y++;
			if (this->Myboard->Get(XY.x, XY.y) == SHIP)
				orient = HORIZONT;
			return XY;
		}
		else if (this->HitEnemyboard->Get(XY.x, XY.y - 1) == 0) {
			XY.y--;
			if (this->Myboard->Get(XY.x, XY.y) == SHIP)
				orient = HORIZONT;
            		return XY;
		}
		else if (this->HitEnemyboard->Get(XY.x + 1, XY.y) == 0) {
			XY.x++;
			if (this->Myboard->Get(XY.x, XY.y) == SHIP)
				orient = VERTICAL;
			return XY;
		}
		else
        	{
			XY.x--;
			if (this->Myboard->Get(XY.x, XY.y) == SHIP)
				orient = VERTICAL;
			return XY;
		}
	}

	else if (orient == VERTICAL) 
	{
		if (this->HitEnemyboard->Get(XY.x + 1, XY.y) == 0)
       		{
			XY.x++;
			return XY;
        	}
		else if (this->HitEnemyboard->Get(XY.x - 1, XY.y) == 0)
        	{
			XY.x--;
			return XY;
        	}
	}

	else 
	{
		if (this->HitEnemyboard->Get(XY.x, XY.y + 1) == 0)
        	{
			XY.y++;
            		return XY;
        	}
		else //if (this->HitEnemyboard->Get(XY.x, XY.y - 1) == 0)
        	{
			XY.y--;
            		return XY;
        	}
	}
	return XY;
}

// computer turn
int Player::enemy_turn()
{
	Myboard->Print();
	Enemyboard->Print();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	coords XY;

	if (Last_hit.x != 0 && Last_hit.y != 0)
   	{
		XY = target(Last_hit, this->orient_last_hit);
	}
	else 
	{
		int size = 0;
		std::vector<coords> free = HitEnemyboard->free_cells(&size);
		if(size != 0)
            		XY = free[rand() % size];
       		else
            		return PLAYER_LOSE;
	}
    	int status = hit(XY, *Myboard, *HitEnemyboard);
	if(status == HIT)
	{
		HitEnemyboard->Set(XY.x, XY.y, HIT_SHIP);
		this->Last_hit = XY;
		return COMP_SHOOT_AGAIN;
	}
	else if (status == MISS)
	{
		HitEnemyboard->Set(XY.x, XY.y, HIT_MISS);
		
		// If computer miss - player shoots next
		return COMP_MISS;
	}
	else if (status == KILL)
	{
		HitEnemyboard->Set(XY.x, XY.y, HIT_SHIP);
		
		// Add dead zone around ship
		HitEnemyboard->dead_zone(XY, SHIP);
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

// player turn
int Player::turn(coords XY)
{
	Hitboard->Print();
   	if(Hitboard->Get(XY.x, XY.y) != 0)
		return PLAYER_SHOOT_AGAIN;
	
	int status = hit(XY, *Enemyboard, *Hitboard);
	if (status == HIT)
	{
		Hitboard->Set(XY.x, XY.y, HIT_SHIP);
		
		// If hit, you can continue shooting
		return PLAYER_SHOOT_AGAIN;
	}
	else if (status == MISS)
	{
		Hitboard->Set(XY.x, XY.y, HIT_MISS);
		
		// If miss - your opponent shoots next
		return PLAYER_MISS;
	}
	else if (status == KILL)
	{
		Hitboard->Set(XY.x, XY.y, HIT_SHIP);
		
		// Add dead zone around ship
		Hitboard->dead_zone(XY, SHIP);
		++kills;
		
		// If hit, you can continue shooting
		// if you haven't won yet
		if (kills < ship_number)
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
	while (true)
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
			i++;
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
			i++;
		}
	}

	return KILL;
}

// EXTRA MODE

// player turn at extra fan mode
int Player::extra_turn(coords XY) 
{
	std::cout << extra_hit(XY) << "KILLS" << std::endl;

	if (kills < ship_number)
		return PLAYER_MISS;
	else return PLAYER_WIN;

	return ERR_PLACE;
}

// computer turn at extra fan mode:
// = 9 ordinary computer turns
int Player::extra_enemy_turn() 
{

	int size = 0;
	std::vector<coords> free = HitEnemyboard->free_cells(&size);
	coords XY = free[rand() % size];
	coords XY_ = XY;
	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);
	XY_.x = XY.x + 1;
	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);
	XY_.x = XY.x - 1;
	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);

	XY_.y = XY.y + 1;
	XY_.x = XY.x;

	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);
	XY_.x = XY.x + 1;
	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);
	XY_.x = XY.x - 1;
	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);

	XY_.y = XY.y - 1;
	XY_.x = XY.x;

	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);
	XY_.x = XY.x + 1;
	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);
	XY_.x = XY.x - 1;
	if (Myboard->Get(XY_.x, XY_.y) != POISON)
		eeturn(XY_);

	return COMP_MISS;
}

// player turn at extra fan mode:
// = 9 ordinary player turns
int Player::extra_hit(coords XY) {
	coords XY_ = XY;
	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	XY_.x = XY.x + 1;
	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	XY_.x = XY.x - 1;
	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	Hitboard->Print();
	XY_.y = XY.y + 1;
	XY_.x = XY.x;

	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	XY_.x = XY.x + 1;
	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	XY_.x = XY.x - 1;
	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	Hitboard->Print();
	XY_.y = XY.y - 1;
	XY_.x = XY.x;

	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	XY_.x = XY.x + 1;
	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	XY_.x = XY.x - 1;
	if (Enemyboard->Get(XY_.x, XY_.y) != POISON)
		turn(XY_);
	Hitboard->Print();

	return kills;
}

// computer turn
int Player::eeturn(coords XY)
{
	int status = hit(XY, *Myboard, *HitEnemyboard);
	if (status == HIT)
	{
		HitEnemyboard->Set(XY.x, XY.y, HIT_SHIP);
		
		// If hit, you can continue shooting
		return COMP_SHOOT_AGAIN;
	}
	else if (status == MISS)
	{
		HitEnemyboard->Set(XY.x, XY.y, HIT_MISS);
		
		// If miss - your opponent shoots next
		return COMP_MISS;
	}
	else if (status == KILL)
	{
		HitEnemyboard->Set(XY.x, XY.y, HIT_SHIP);
		
		// Add dead zone around ship
		HitEnemyboard->dead_zone(XY, SHIP);
		++enemy_kills;
		
		// If hit, you can continue shooting
		// if you haven't won yet
		if (enemy_kills < ship_number)
			return COMP_SHOOT_AGAIN;
		else return PLAYER_LOSE;
	}
	else return ERR_PLACE;
}

