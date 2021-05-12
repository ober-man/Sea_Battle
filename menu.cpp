#include "menu.h"
Menu::Menu()
{
    button_color = Color(126, 0, 0);
    main_color = Color(126, 181, 221);
    start = new Button("images/string_start.png", 30, 10, button_color);
    settings = new Button("images/string_settings.png", 30, 70, button_color);
    exit = new Button("images/string_exit.png", 30, 130, button_color);
    comp_game = new Button("images/comp_game.png", 190, 370, button_color);
    super_fan = new Button("images/super_fan.png", 1350, 350, button_color);
    background = new MapWidget(0, 0, 1900, 900, "images/background.png");
    game_mode = new MapWidget(0, 0, 1900, 900, "images/game_mode.png");
    settings_map = new MapWidget(0, 0, 1900, 900, "images/settings.png");
}

Menu::~Menu()
{
    delete start;
    delete settings;
    delete exit;
    delete comp_game;
    delete super_fan;
    delete background;
    delete game_mode;
    delete settings_map;
}

int Menu::run(RenderWindow& window)
{
	while(window.isOpen())
    {
        start->SetColor(button_color);
        settings->SetColor(button_color);
        exit->SetColor(button_color);
        Event event;
        while(window.pollEvent(event))
            if(event.type == Event::Closed)
                window.close();
        window.clear(main_color);

        if(IntRect(30, 10, 180, 40).contains(Mouse::getPosition(window)))
        {
            start->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                return GameMode(window);
        }
        if(IntRect(30, 70, 200, 40).contains(Mouse::getPosition(window)))
        {
            settings->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                SettingsWindow(window);
        }
        if(IntRect(30, 130, 100, 40).contains(Mouse::getPosition(window)))
        {
            exit->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                window.close();
        }
        background->Draw(window);
        start->Draw(window);
        settings->Draw(window);
        exit->Draw(window);
        window.display();
    }
    return ERR_MODE;
}

int Menu::GameMode(RenderWindow& window)
{
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
            if(event.type == sf::Event::Closed)
                window.close();

        comp_game->SetColor(button_color);
        super_fan->SetColor(button_color);
        window.clear(main_color);

        if(IntRect(190, 370, 300, 200).contains(Mouse::getPosition(window)))
        {
            comp_game->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                return COMP_GAME;
        }
        /*if(IntRect(590, 370, 300, 200).contains(Mouse::getPosition(window)))
        {
            comp_game->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                return COMP_GAME;
        }*/
        if(IntRect(1350, 350, 300, 200).contains(Mouse::getPosition(window)))
        {
            super_fan->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                return SUPER_FAN;
        }
        game_mode->Draw(window);
        comp_game->Draw(window);
        super_fan->Draw(window);
        window.display();
    }
    return ERR_MODE;
}

void Menu::SettingsWindow(RenderWindow& window)
{
    Button escape{"images/string_exit.png", 1630, 830, button_color}; // may be change
    while(window.isOpen())
    {
        escape.SetColor(button_color);
        Event event;
        while(window.pollEvent(event))
            if(event.type == Event::Closed)
                window.close();
            else if(event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)
                return;
        if(IntRect(1630, 830, 100, 40).contains(Mouse::getPosition(window)))
        {
            escape.SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                return;
        }
        settings_map->Draw(window);
        escape.Draw(window);
        window.display();
    }
}

