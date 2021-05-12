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
        int random_ships();
        int dead_zone(coords XY, int check);
        int personal_ships();
        int add_extra_hit(coords XY);
        void Print() const;
};
