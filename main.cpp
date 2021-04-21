#define GRAPHICS

#ifdef GRAPHICS
#include<SFML/Graphics.hpp>
#include"Application.h"

int main()
{
    Application app;
    app.Run();

    return 0;
}
#endif

#ifndef GRAPHICS
#include "game.h"
int main() {
	Board game;
	game.random_ships(game.Enemyboard);
	game.print(game.Hitboard);
	game.print(game.Enemyboard);
	while(1)
    {
        coords XY;
        std::cin >> XY.x;
        std::cin >> XY.y;
        game.turn(XY);
        game.print(game.Hitboard);
        game.print(game.Enemyboard);
    }
	return 0;
}
#endif
