
//
#define GRAPHICS

#ifdef GRAPHICS
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
int main()
{
	Player man;
	man.Myboard->random_ships();
	man.Enemyboard->random_ships();
	man.print(*man.Hitboard);
	man.print(*man.Enemyboard);
	while(1)
    {
        coords XY;
        std::cin >> XY.x;
        std::cin >> XY.y;
        man.turn(XY);
        man.print(*man.Hitboard);
        man.print(*man.Enemyboard);
    }
	return 0;
}
#endif
