#pragma once
#include"SFML/Audio.hpp"
#include"map.h"
#include"menu.h"
#include "game.h"

class Application
{
     private:
        RenderWindow *window;
        MapWidget *game;
        Button *random,
	       *lose,
	       *win;
        Color button_color,
              main_color;
        Music pirate_music;
        Menu menu;
        Player *man;
	
        void StartGame(Sprite s, int mode);
        void DrawBoards(Sprite s, int mode);
        void DrawTable(Sprite s, int mode);

     public:
        Application();
        ~Application();
        void Run();
};
