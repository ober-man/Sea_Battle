#pragma once
#include"errors.h"
#include"button.h"
#include<iostream>

using namespace sf;

class Menu
{
    private:
        Button *start,
               *settings,
               *exit,
               *comp_game,
               *super_fan;
        MapWidget *background,
		  *game_mode,
                  *settings_map;
        Color main_color,
              button_color;
        void SettingsWindow(RenderWindow& window);
        int GameMode(RenderWindow& window);
	
    public:
        Menu();
        void run(RenderWindow& window);
        ~Menu();
};
