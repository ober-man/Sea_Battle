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
        Texture random_texture;
        Menu m;
        void StartGame(RenderWindow* window);

     public:
        Application();
        ~Application();
        void Run();
};

Application::Application()
{
    window = new sf::RenderWindow(sf::VideoMode(1900, 900), "Sea Battle");
    // Style::Fullscreen - 3 arg, params: 1040, 610
    /*Music pirateMusic;
    pirateMusic.setLoop(true);
    if (!pirateMusic.openFromFile("sounds/pirate.wav"))
      exit(WRONG_MUSIC);
    pirateMusic.play();*/
    mapwidget = new MapWidget(0, 0, 1900, 900, "images/game_begin.png");
    if(!random_texture.loadFromFile("images/random.png"))
        exit(WRONG_PICTURE);
}

void Application::Run()
{
    m.run(*window);
    Sprite random(random_texture);
    random.setPosition(1450, 750);
    while(window->isOpen())
    {
        sf::Event event;
        while(window->pollEvent(event))
            if(event.type == sf::Event::Closed)
                window->close();

        random.setColor(Color(126, 0, 0));
        window->clear(Color(126, 181, 221));

        if (IntRect(1450, 750, 1600, 120).contains(Mouse::getPosition(*window)))
        {
            random.setColor(Color::Blue);
            if (Mouse::isButtonPressed(Mouse::Left))
                StartGame(window);
        }

        mapwidget->Draw(window);
        window->draw(random);
        window->display();
    }
}

Application::~Application()
{
    if (window != nullptr)
        delete window;

    if (mapwidget != nullptr)
        delete mapwidget;
}

void Application::StartGame(RenderWindow* window)
{
    Texture cell;
    if(!cell.loadFromFile("images/cell.png"))
        exit(WRONG_PICTURE);
    Sprite s(cell);
    Board b;
    b.random_ships(b.Myboard);
    b.random_ships(b.Enemyboard);

    int** MyEmptyBoard = new int* [LSIZE];
	for (int i = 0; i < LSIZE; i++)
		MyEmptyBoard[i] = new int[LSIZE];
    for (int i = 0; i < LSIZE; i++)
		for (int j = 0; j < LSIZE; j++)
            if(b.Myboard[i][j] == 2)
                MyEmptyBoard[i][j] = 2;
            else
                MyEmptyBoard[i][j] = 10;

    while(window->isOpen())
    {
        Vector2i pos = Mouse::getPosition(*window);
        int x = (pos.x) / len;
        int y = (pos.y) / len;

        Event event;
        while(window->pollEvent(event))
        {
            if(event.type == Event::Closed)
                window->close();
            if(event.type == Event::MouseButtonPressed)
				if(event.key.code == Mouse::Left)
                    MyEmptyBoard[x][y] = b.Myboard[x][y];
        }

        window->clear(Color::White);
        for(int i = 0; i < LSIZE; i++)
			for(int j = 0; j < LSIZE; j++)
			{
			    if(MyEmptyBoard[i][j] == 10)
                    s.setTextureRect(IntRect(0, 0, len, len));
			    else if(MyEmptyBoard[i][j] == 2)
                    s.setTextureRect(IntRect(3 * len, 0, len, len));
                else if(MyEmptyBoard[i][j] == 0)
                    s.setTextureRect(IntRect(2 * len, 0, len, len));
                else
                    s.setTextureRect(IntRect(len, 0, len, len));
				s.setPosition(i*len, j*len);
				window->draw(s);
			}

        /*for(int i = 0; i < LSIZE; i++)
			for(int j = 0; j < LSIZE; j++)
			{
			    if(b.Enemyboard[i][j] == 2)
                    s.setTextureRect(IntRect(3 * len, 0, len, len));
                else
                    s.setTextureRect(IntRect(0, 0, len, len));
				s.setPosition(1500 + i*len, 1500 + j*len);
				window->draw(s);
			}*/

        window->display();
    }
    for (int i = 0; i < LSIZE; i++) {
		delete MyEmptyBoard[i];
	}
    delete MyEmptyBoard;
}
