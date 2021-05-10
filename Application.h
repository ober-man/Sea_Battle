#pragma once
#include"SFML/Audio.hpp"
#include"map.h"
#include"menu.h"
#include "game.h"

class Application
{
     private:
        RenderWindow *window;
        MapWidget *ship_position,
                  *game;
        Button *random;
        Color button_color,
              main_color;
        Menu menu;
        void StartGame(RenderWindow* window);
        void DrawBoards(RenderWindow* window, Player& man, Sprite s);

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
    ship_position = new MapWidget(0, 0, 1900, 900, "images/position.png");
    game = new MapWidget(0, 0, 1900, 900, "images/table.jpg");
    button_color = Color(126, 0, 0);
    main_color = Color(126, 181, 221);
    random = new Button("images/random.png", 1450, 750, button_color);
}

Application::~Application()
{
    delete window;
    delete ship_position;
    delete game;
    delete random;
}

void Application::Run()
{
    menu.run(*window); // return value = game mode
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

        ship_position->Draw(*window);
        random->Draw(*window);
        window->display();
    }
}

void Application::StartGame(RenderWindow* window)
{
    Texture cell;
    if(!cell.loadFromFile("images/cell.png"))
        exit(WRONG_PICTURE);
    Sprite s(cell);

    Player man;
    man.random_position();
    int status = 10;

    while(window->isOpen())
    {
        if(status > 0)
        {
            if(status == PLAYER_WIN)
                return; // make a window

            Vector2i pos = Mouse::getPosition(*window);
            int x_my = (pos.x - x_0) / len;
            int y_my = (pos.y - y_0) / len;
            coords XY_my = {x_my, y_my};

            Event event;
            while(window->pollEvent(event))
            {
                if(event.type == Event::Closed)
                    window->close();
                if(event.type == Event::MouseButtonPressed)
                    if(event.key.code == Mouse::Left)
                        if(x_my > 0 && y_my > 0 && x_my < SIZE && y_my < SIZE)
                        {
                            // add condition if user press killed cell
                            status = man.turn(XY_my);
                            std::cout << status << std::endl;
                        }
            }
            game->Draw(*window);
            //window->clear(Color::White);
            DrawBoards(window, man, s);
            window->display();
        }
        else // status < 0
        {
            if(status == PLAYER_LOSE)
                return;
            status = man.enemy_turn();
            std::cout << status << std::endl;

            Event event;
            while(window->pollEvent(event))
            {
                if(event.type == Event::Closed)
                    window->close();
            }
            game->Draw(*window);
            //window->clear(Color::White);
            DrawBoards(window, man, s);
            window->display();
        }
    }
}

void Application::DrawBoards(RenderWindow* window, Player& man, Sprite s)
{
    // Upper left corners
    s.setTextureRect(IntRect(len, 0, len, len));
    s.setPosition(x_0, y_0);
    window->draw(s);
    s.setPosition(x_2, y_2);
    window->draw(s);

    for(int i = 1; i < SIZE; i++)
    {
        s.setTextureRect(IntRect((i + 3) * len, 0, len, len));
        s.setPosition(i*len + x_0, y_0);
        window->draw(s);
        s.setPosition(i*len + x_2, y_2);
        window->draw(s);
        s.setPosition(x_0, i*len + y_0);
        window->draw(s);
        s.setPosition(x_2, i*len + y_2);
        window->draw(s);
    }
    for(int i = 1; i < LSIZE; i++)
        for(int j = 1; j < LSIZE; j++)
        {
            if(man.HitEnemyboard->Get(i, j) == POISON)
                continue;
                //s.setTextureRect(IntRect(0, 0, len, len));
            else if(man.HitEnemyboard->Get(i, j) == HIT_SHIP)
                s.setTextureRect(IntRect(2 * len, 0, len, len));
            else if(man.HitEnemyboard->Get(i, j) == HIT_MISS)
                s.setTextureRect(IntRect(3 * len, 0, len, len));
            else
                s.setTextureRect(IntRect(1 * len, 0, len, len));
            s.setPosition(i*len + x_2, j*len + y_2);
            window->draw(s);
        }

    for(int i = 1; i < LSIZE; i++)
        for(int j = 1; j < LSIZE; j++)
        {
            if(man.Hitboard->Get(i, j) == POISON)
                continue;
                //s.setTextureRect(IntRect(0, 0, len, len));
            else if(man.Hitboard->Get(i, j) == HIT_SHIP)
                s.setTextureRect(IntRect(2 * len, 0, len, len));
            else if(man.Hitboard->Get(i, j) == HIT_MISS)
                s.setTextureRect(IntRect(3 * len, 0, len, len));
            else
                s.setTextureRect(IntRect(1 * len, 0, len, len));
            s.setPosition(i*len + x_0, j*len + y_0);
            window->draw(s);
        }
}
