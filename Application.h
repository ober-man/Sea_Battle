#pragma once
#include"SFML/Audio.hpp"
#include"map.h"
#include"menu.h"
#include "game.h"

class Application
{
     private:
        RenderWindow *window;
        MapWidget *mapwidget;
        Button *random;
        Color button_color,
              main_color;
        Menu menu;
        void StartGame(RenderWindow* window);

     public:
        Application();
        ~Application();
        void Run();
};

Application::Application()
{
    window = new sf::RenderWindow(sf::VideoMode(1900, 900), "Sea Battle");
    // Style::Fullscreen - 3 arg, params: 1040, 610 (1900, 900) getFullscreenModes getDesktopMode
    /*Music pirateMusic;
	pirateMusic.setLoop(true);
	if (!pirateMusic.openFromFile("sounds/pirate.wav"))
		exit(WRONG_MUSIC);
	pirateMusic.play();*/
    mapwidget = new MapWidget(0, 0, 1900, 900, "images/game_begin.png");
    button_color = Color(126, 0, 0);
    main_color = Color(126, 181, 221);
    random = new Button("images/random.png", 1450, 750, button_color);
}

void Application::Run()
{
    menu.run(*window);
    while(window->isOpen())
    {
        sf::Event event;
        while(window->pollEvent(event))
            if(event.type == sf::Event::Closed)
                window->close();

        random->SetColor(button_color);
        window->clear(main_color);

        if(IntRect(1450, 750, 1600, 120).contains(Mouse::getPosition(*window)))
        {
            random->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                StartGame(window);
        }

        mapwidget->Draw(*window);
        random->Draw(*window);
        window->display();
    }
}

Application::~Application()
{
    delete window;
    delete mapwidget;
    delete random;
}

void Application::StartGame(RenderWindow* window)
{
    Texture cell;
    if(!cell.loadFromFile("images/cell.png"))
        exit(WRONG_PICTURE);
    Sprite s(cell);

    Player man;
    man.random_position();
    int status = 0;

    while(window->isOpen())
    {
        Vector2i pos = Mouse::getPosition(*window);
        int x_my = (pos.x - x_0) / len;
        int y_my = (pos.y - y_0) / len;
        coords XY_my = {x_my, y_my};
        int x_enemy = (pos.x - x_2) / len;
        int y_enemy = (pos.y - y_2) / len;
        coords XY_enemy = {x_enemy, y_enemy};

        Event event;
        while(window->pollEvent(event))
        {
            if(event.type == Event::Closed)
                window->close();
            if(event.type == Event::MouseButtonPressed)
				if(event.key.code == Mouse::Left)
                {
                    if(x_my > 0 && y_my > 0 && x_my < LSIZE - 1 && y_my < LSIZE - 1)
                        // add condition if user press killed cell
                        status = man.turn(XY_my);
                        //MyEmptyBoard.Set(x_my, y_my, man.Myboard->Get(x_my, y_my));
                    if(x_enemy > 0 && y_enemy > 0 && x_enemy < LSIZE - 1 && y_enemy < LSIZE - 1)
                        status = man.turn(XY_enemy);
                        //EnemyEmptyBoard.Set(x_enemy, y_enemy, man.Myboard->Get(x_enemy, y_enemy));
                }
        }
        window->clear(Color::White);

        for(int i = 0; i < LSIZE; i++)
			for(int j = 0; j < LSIZE; j++)
			{
				if(man.Hitboard->Get(i, j) == POISON)
					s.setTextureRect(IntRect(0, 0, len, len));
				else if(man.Hitboard->Get(i, j) == HIT_SHIP)
					s.setTextureRect(IntRect(2 * len, 0, len, len));
				else if(man.Hitboard->Get(i, j) == HIT_MISS)
					s.setTextureRect(IntRect(3 * len, 0, len, len));
				else
					s.setTextureRect(IntRect(1 * len, 0, len, len));
				s.setPosition(i*len + x_0, j*len + y_0);
				window->draw(s);
			}

        for(int i = 0; i < LSIZE; i++)
			for(int j = 0; j < LSIZE; j++)
			{
			    if(man.HitEnemyboard->Get(i, j) == POISON)
                    s.setTextureRect(IntRect(0, 0, len, len));
                else if(man.HitEnemyboard->Get(i, j) == HIT_SHIP)
                    s.setTextureRect(IntRect(2 * len, 0, len, len));
                else if(man.HitEnemyboard->Get(i, j) == HIT_MISS)
                    s.setTextureRect(IntRect(3 * len, 0, len, len));
                else
                    s.setTextureRect(IntRect(1 * len, 0, len, len));
				s.setPosition(i*len + x_2, j*len + y_2);
				window->draw(s);
			}

        window->display();
    }
}
