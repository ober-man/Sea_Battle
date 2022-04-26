#pragma once
#include"board.h"
#include <chrono>
#include <thread>
#define RANDOM

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
        int eeturn(coords XY);

    public:
        Board* Myboard;
        Board* Enemyboard;
        Board* Hitboard;
        Board* HitEnemyboard;

        Player();
        ~Player();
        int Get_kills() const;
        int Get_enemy_kills() const;
	int check_ship_number();
        int turn(coords XY);
        int enemy_turn();
        int extra_turn(coords XY);
        int extra_enemy_turn();
        int random_position();
        coords enter_XY();
};
